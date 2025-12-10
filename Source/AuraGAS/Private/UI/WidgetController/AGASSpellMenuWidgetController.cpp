// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASSpellMenuWidgetController.h"

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
}

void UAGASSpellMenuWidgetController::SelectedAbility(UAGASUserWidget* AbilityButton)
{
	OnAbilityButtonSelected.Broadcast(AbilityButton);
}

void UAGASSpellMenuWidgetController::OnSpendPointButtonPressed(const FGameplayTag& AbilityTag)
{
	AGASAbilitySystemComponent->ServerSpendSpellPoint(AbilityTag);
}

