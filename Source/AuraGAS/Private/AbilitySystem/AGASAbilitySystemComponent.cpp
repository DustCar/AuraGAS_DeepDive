// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AGASGameplayTags.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AGASDamageGameplayAbility.h"
#include "AbilitySystem/Abilities/AGASGameplayAbility.h"
#include "AbilitySystem/Data/AGASAbilityInfo.h"
#include "AuraGAS/AGASLogChannels.h"
#include "Interaction/AGASPlayerInterface.h"

void UAGASAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);
	
	// Only run the code on locally controlled actors since it pertains to UI
	const bool bIsLocallyControlled = AbilityActorInfo->IsLocallyControlled();
	if (!bIsLocallyControlled) return;
	
	// If ability becomes eligible (i.e. when player levels up) then we broadcast that change. 
	// This time we do it when we give the ability to make the change more consistent rather than
	// calling ClientUpdateStatuses in UpdateAbilityStatuses
	const FGameplayTag StatusTag = GetStatusTagFromSpec(AbilitySpec);
	if (StatusTag.MatchesTagExact(TAG_Abilities_Status_Eligible))
	{
		AbilityStatusChanged.Broadcast(GetAbilityTagFromSpec(AbilitySpec), StatusTag, AbilitySpec.Level);
	}
}

void UAGASAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::ClientEffectApplied);
}

void UAGASAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAGASGameplayAbility* AGASAbility = Cast<UAGASGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AGASAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(TAG_Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenSignature.Broadcast();
}

void UAGASAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAGASAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);

			if (!AbilitySpec.IsActive()) return;
			UGameplayAbility* PrimaryInstance = AbilitySpec.GetPrimaryInstance();
			if (PrimaryInstance)
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, PrimaryInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
			}
		}
	}
}

void UAGASAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);

			if (AbilitySpec.IsActive()) return;
			TryActivateAbility(AbilitySpec.Handle);

		}
	}
}

void UAGASAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			
			if (!AbilitySpec.IsActive()) return;
			// NOTE: GetPrimaryInstance() is only valid on InstancedPerActor abilities
			// for InstancedPerExecution abilities, GetAbilityInstances() would be used that would return an array of GAs
			// this is for the fix of AbilitySpec.ActivationInfo being deprecated in 5.5 onward
			UGameplayAbility* PrimaryInstance = AbilitySpec.GetPrimaryInstance();
			if (PrimaryInstance)
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, PrimaryInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
			}
		}
	}
}

void UAGASAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	// stops the ASC from removing abilities while iterating through abilities
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAGAS, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__)
		}
	}
}

void UAGASAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (!GetAvatarActor()->Implements<UAGASPlayerInterface>()) return;

	if (IAGASPlayerInterface::Execute_GetAttributePointsOnPlayerState(GetAvatarActor()) > 0)
	{
		ServerUpgradeAttribute(AttributeTag);
	}
}

void UAGASAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.0f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if (GetAvatarActor()->Implements<UAGASPlayerInterface>())
	{
		IAGASPlayerInterface::Execute_AddToAttributePointsOnPlayerState(GetAvatarActor(), -1.0f);
	}
}

FGameplayTag UAGASAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability == nullptr) return FGameplayTag();
	
	for (FGameplayTag Tag : AbilitySpec.Ability->AbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAGASAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability == nullptr) return FGameplayTag();
	
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAGASAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability == nullptr) return FGameplayTag();
	
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

bool UAGASAbilitySystemComponent::AbilityIsAlreadyEquipped(const FGameplayAbilitySpec& AbilitySpec)
{
	return AbilitySpec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

FGameplayAbilitySpec* UAGASAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability->AbilityTags)
		{
			if (Tag.MatchesTagExact(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

bool UAGASAbilitySystemComponent::IsPassiveAbility(const FGameplayTag& AbilityTag) const
{
	const UAGASAbilityInfo* AbilityInfo = UAGASAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (AbilityInfo)
	{
		const FAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		const FGameplayTag LocalAbilityType = Info.TypeTag;
		
		return LocalAbilityType.MatchesTagExact(TAG_Abilities_Type_Passive);
	}
	
	return false;
}

FGameplayAbilitySpec* UAGASAbilitySystemComponent::GetSpecFromInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			return &AbilitySpec;
		}
	}
	
	return nullptr;
}

void UAGASAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAGASAbilityInfo* AbilityInfo = UAGASAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (AbilityInfo == nullptr)
	{
		UE_LOG(LogAGAS, Error, TEXT("Could not find AbilityInfo on GameInstance. Please check if AbilityInfo is set or check if GameMode is set."))
		return;
	}
	
	// loop through all possible abilities in the spell menu tree and update the status tags
	// for the ones within the level requirement by giving the ability to the player
	for (const FAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(TAG_Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAGASAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UAGASPlayerInterface>())
		{
			IAGASPlayerInterface::Execute_AddToSpellPointsOnPlayerState(GetAvatarActor(), -1);
		}

	    FGameplayTag Status = GetStatusTagFromSpec(*AbilitySpec);
	    if (Status.MatchesTagExact(TAG_Abilities_Status_Eligible))
	    {
		    AbilitySpec->DynamicAbilityTags.RemoveTag(TAG_Abilities_Status_Eligible);
	    	AbilitySpec->DynamicAbilityTags.AddTag(TAG_Abilities_Status_Unlocked);
	    	Status = TAG_Abilities_Status_Unlocked;
	    }
		else if (Status.MatchesTagExact(TAG_Abilities_Status_Equipped) || Status.MatchesTagExact(TAG_Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		
		ClientUpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

bool UAGASAbilitySystemComponent::GetAbilityDescriptionsFromTagAndLevel(const FGameplayTag& AbilityTag, int32 Level, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	const UAGASAbilityInfo* AbilityInfo = UAGASAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAGASGameplayAbility* AGASAbility = Cast<UAGASGameplayAbility>(AbilitySpec->Ability))
		{
			// Note: Previously used AbilitySpec->Level for level but is too inconsistent with client so we pass one in
			// Note for formats: Formats are for damage abilities and will only format if ability is a damage GA, else
			// it's normal
			const FString FormattedDescription = UAGASDamageGameplayAbility::FormatDamageAbilityDescription(Level, Info.AbilityDescription.ToString(), AGASAbility);
			const FString FormattedNextLvlDescription = UAGASDamageGameplayAbility::FormatDamageAbilityDescription(Level, Info.NextLevelDescription.ToString(), AGASAbility);
			
			OutDescription = FString::Printf(
				TEXT(
				// Ability Name
				"<Title>%s </>\n"
				
				// Level, Mana cost, and Cooldown
				"<SubTitle>Level: </><Level>%d</>\n"
				"<SubTitle>Mana: </><ManaCost>%d</>\n"
				"<SubTitle>Cooldown: </><Cooldown>%.1f</> s\n"
				
				// Ability info body that gets formatted and obtained from ability info
				"\n"
				"%s\n"
				"\n"
				),
				*Info.AbilityName.ToString(),
				Level,
				AGASAbility->GetRoundedManaCost(Level),
				AGASAbility->GetCooldown(Level),
				*FormattedDescription
			);
			OutNextLevelDescription = FString::Printf(
				TEXT(
				// Same as Ability description but includes the previous and next level iteration
				"<Title>%s </>\n"
				"<SubTitle>Level: </><OldValue>%d</> > <Level>%d</>\n"
				"<SubTitle>Mana: </><OldValue>%d</> > <ManaCost>%d</>\n"
				"<SubTitle>Cooldown: </><OldValue>%.1f</> > <Cooldown>%.1f</> s\n"
				"\n"
				"%s\n"
				"\n"
				),
				*Info.AbilityName.ToString(),
				Level,
				Level + 1,
				AGASAbility->GetRoundedManaCost(Level),
				AGASAbility->GetRoundedManaCost(Level + 1),
				AGASAbility->GetCooldown(Level),
				AGASAbility->GetCooldown(Level + 1),
				*FormattedNextLvlDescription
			);
			return true;
		}
	}
	
	// here we consider the ability locked if no spec is found
	// also, clear ability description if spell globe does not have an ability assigned
	if (AbilityTag.IsValid())
	{
		OutDescription = UAGASGameplayAbility::GetLockedDescription(Info.LevelRequirement);
	}
	else
	{
		OutDescription = FString();
	}
	OutNextLevelDescription = FString();
	return false;
}

void UAGASAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& NewInputTag)
{
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	if (AbilitySpec)
	{
		const FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		const bool bStatusValid = StatusTag.MatchesTagExact(TAG_Abilities_Status_Equipped) || StatusTag.MatchesTagExact(TAG_Abilities_Status_Unlocked);
		
		if (bStatusValid)
		{
			// first we have to clear any ability that is currently equipped in the NewInputTag globe
			// we'll "unequip" the ability by removing the input tag from its Dynamic tags and changing its Status to unlocked
			FGameplayAbilitySpec* CurrentEquippedAbilitySpec = GetSpecFromInputTag(NewInputTag);
			if (CurrentEquippedAbilitySpec)
			{
				const FGameplayTag& CurrentEquippedAbilityTag = GetAbilityTagFromSpec(*CurrentEquippedAbilitySpec);
				CurrentEquippedAbilitySpec->DynamicAbilityTags.RemoveTag(NewInputTag);
				CurrentEquippedAbilitySpec->DynamicAbilityTags.RemoveTag(TAG_Abilities_Status_Equipped);
				CurrentEquippedAbilitySpec->DynamicAbilityTags.AddTag(TAG_Abilities_Status_Unlocked);
				
				// if the old ability is a passive then deactivate that ability
				if (IsPassiveAbility(CurrentEquippedAbilityTag))
				{
					DeactivatePassiveAbility.Broadcast(CurrentEquippedAbilityTag);
					MulticastActivatePassiveEffect(CurrentEquippedAbilityTag, false);
				}
				MarkAbilitySpecDirty(*CurrentEquippedAbilitySpec);
				
				ClientBroadcastOnAbilityUnequipped(CurrentEquippedAbilityTag);
			}
			
			// if a passive ability hasn't been equipped yet then try to activate it
			if (!AbilityIsAlreadyEquipped(*AbilitySpec))
			{
				if (IsPassiveAbility(AbilityTag))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
			}
			
			// cache the old input tag for broadcast
			const FGameplayTag OldInputTag = GetInputTagFromSpec(*AbilitySpec);
			// we'll clear the input tag for the current ability spec that we are trying to equip then we set the new one
			ClearAbilityInputTag(AbilitySpec);
			AbilitySpec->DynamicAbilityTags.AddTag(NewInputTag);
			// make sure to change the status of the ability spec to be equipped rather than unlocked if first time
			if (StatusTag.MatchesTagExact(TAG_Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(TAG_Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(TAG_Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
			
			ClientBroadcastOnAbilityEquipped(AbilityTag, OldInputTag, NewInputTag);
		}
	}
}

void UAGASAbilitySystemComponent::ClearAbilityInputTag(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag InputTag = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(InputTag);
	MarkAbilitySpecDirty(*Spec);
}

void UAGASAbilitySystemComponent::ClientBroadcastOnAbilityEquipped_Implementation(const FGameplayTag& AbilityTag,
                                                                                  const FGameplayTag& OldInputTag, const FGameplayTag& NewInputTag)
{
	OnAbilityEquipped.Broadcast(AbilityTag, OldInputTag, NewInputTag);
}

void UAGASAbilitySystemComponent::ClientBroadcastOnAbilityUnequipped_Implementation(const FGameplayTag& AbilityTag)
{
	OnAbilityUnequipped.Broadcast(AbilityTag);
}

void UAGASAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bActivate)
{
	ActivatePassiveEffect.Broadcast(AbilityTag, bActivate);
}

void UAGASAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenSignature.Broadcast();
	}
}

void UAGASAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UAGASAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

