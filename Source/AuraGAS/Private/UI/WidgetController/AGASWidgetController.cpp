// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASWidgetController.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/Data/AGASAbilityInfo.h"

void UAGASWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	AGASPlayerController = WCParams.PlayerController;
	AGASPlayerState = WCParams.PlayerState;
	AGASAbilitySystemComponent = WCParams.AbilitySystemComponent;
	AGASAttributeSet = WCParams.AttributeSet;
}

void UAGASWidgetController::BroadcastInitialValues()
{
	
}

void UAGASWidgetController::BindCallbacksToDependencies()
{
	
}

void UAGASWidgetController::BroadcastAbilityInfo()
{
	if (!AGASAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility AbilitiesDelegate;
	AbilitiesDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		// make sure that the given ability is actually a proper ability
		// covers the case that GA_ListenForEvents is an ability but is not one that the player actually uses
		const FGameplayTag AbilityTag = UAGASAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec);
		if (!AbilityTag.IsValid()) return;
		
		FAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		Info.InputTag = UAGASAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});

	AGASAbilitySystemComponent->ForEachAbility(AbilitiesDelegate);
}
