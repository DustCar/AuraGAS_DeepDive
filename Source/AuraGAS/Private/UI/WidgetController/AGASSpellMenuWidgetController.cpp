// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASSpellMenuWidgetController.h"

#include "AGASGameplayTags.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/Data/AGASAbilityInfo.h"
#include "Player/AGASPlayerState.h"

void UAGASSpellMenuWidgetController::BroadcastInitialValues()
{
	// no need to place a check to see if player has been given his Startup Abilities since these globes would hold
	// all abilities no matter the status (Equipped, Locked, etc.)
	BroadcastAbilityInfo();
	
	OnPlayerSpellPointsChangedWidget.Broadcast(AGASPlayerState->GetSpellPoints());
}

void UAGASSpellMenuWidgetController::BindCallbacksToDependencies()
{
	// Similar to Overlay Widget Controller, we need to bind to the AbilitiesGivenSignature in case the client
	// is broadcasting ability info before getting the abilities
	if (!AGASAbilitySystemComponent->bStartupAbilitiesGiven)
	{
		AGASAbilitySystemComponent->AbilitiesGivenSignature.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
	}
	
	// broadcast to AbilityInfoDelegate the new statuses
	AGASAbilitySystemComponent->AbilityStatusChanged.AddLambda(
		[this] (const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if (AbilityInfo)
			{
				FAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				Info.AbilityLevel = NewLevel;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);
	
	AGASPlayerState->OnSpellPointsChangedSignature.AddLambda([this] (int32 NewSpellPoints)
	{
		OnPlayerSpellPointsChangedWidget.Broadcast(NewSpellPoints);
	});
	
	AGASAbilitySystemComponent->OnAbilityEquipped.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& OldInputTag, const FGameplayTag& NewInputTag)
	{
		// before equipping the ability to a new input tag, we will clear the old input tag that the ability was assigned to
		// IF it was previously equipped already
		if (OldInputTag.IsValid())
		{
			FAbilityInfo OldAbilityInfo;
			OldAbilityInfo.AbilityTag = FGameplayTag();
			OldAbilityInfo.InputTag = OldInputTag;
			AbilityInfoDelegate.Broadcast(OldAbilityInfo);
		}
		
		// create a new ability info with an updated input tag for the ability and broadcast it to AbilityInfoDelegate
		FAbilityInfo NewAbilityInfo = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		NewAbilityInfo.InputTag = NewInputTag;
		NewAbilityInfo.StatusTag = TAG_Abilities_Status_Equipped;
		const FGameplayAbilitySpec* AbilitySpec = AGASAbilitySystemComponent->GetSpecFromAbilityTag(AbilityTag);
		NewAbilityInfo.AbilityLevel = AbilitySpec->Level;
		AbilityInfoDelegate.Broadcast(NewAbilityInfo);
		
		OnEquippedAbility.Broadcast();
	});
	
	AGASAbilitySystemComponent->OnAbilityUnequipped.AddLambda([this](const FGameplayTag& AbilityTag)
	{
		// here we will accomplish the similar functionality that we did in OnAbilityEquipped but now with no InputTag
		// and setting it to unlocked rather than equipped
		FAbilityInfo NewAbilityInfo = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		NewAbilityInfo.InputTag = FGameplayTag();
		NewAbilityInfo.StatusTag = TAG_Abilities_Status_Unlocked;
		const FGameplayAbilitySpec* AbilitySpec = AGASAbilitySystemComponent->GetSpecFromAbilityTag(AbilityTag);
		NewAbilityInfo.AbilityLevel = AbilitySpec->Level;
		AbilityInfoDelegate.Broadcast(NewAbilityInfo);
	});
}

void UAGASSpellMenuWidgetController::OnSpendPointButtonPressed(const FGameplayTag& AbilityTag)
{
	AGASAbilitySystemComponent->ServerSpendSpellPoint(AbilityTag);
}

void UAGASSpellMenuWidgetController::EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& NewInputTag)
{
	AGASAbilitySystemComponent->ServerEquipAbility(AbilityTag, NewInputTag);
}

