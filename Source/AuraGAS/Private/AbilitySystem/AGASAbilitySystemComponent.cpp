// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AGASGameplayTags.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AGASGameplayAbility.h"
#include "AbilitySystem/Data/AGASAbilityInfo.h"
#include "AuraGAS/AGASLogChannels.h"
#include "Interaction/AGASPlayerInterface.h"

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

void UAGASAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

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

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
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

void UAGASAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAGASAbilityInfo* AbilityInfo = UAGASAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (AbilityInfo == nullptr)
	{
		UE_LOG(LogAGAS, Error, TEXT("Could not find AbilityInfo on GameMode. Please check if AbilityInfo is set or check if GameMode is set."))
		return;
	}
	
	// loop through all possible abilities in the spell menu tree and update the status tags
	// for the ones within the level requirement
	for (const FAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(TAG_Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			
			// call client RPC to update the menu for both client and server. 
			// need a client one since UpdateAbilityStatuses will only run on server
			ClientUpdateAbilityStatus(Info.AbilityTag, TAG_Abilities_Status_Eligible, 1);
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

