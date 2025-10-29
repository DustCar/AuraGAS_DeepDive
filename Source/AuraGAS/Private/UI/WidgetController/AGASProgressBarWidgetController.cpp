// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASProgressBarWidgetController.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAttributeSet.h"

void UAGASProgressBarWidgetController::BindCallbacksToDependencies()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AGASAttributeSet->GetHealthPointsAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
			{
				OnHealthPointsChanged.Broadcast(Data.NewValue);
				if (bFirstBroadcast)
				{
					OnMaxHealthPointsChanged.Broadcast(AGASAttributeSet->GetMaxHealthPoints());
					bFirstBroadcast = false;
				}
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AGASAttributeSet->GetMaxHealthPointsAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
			{
				if (Data.NewValue > 0)
				{
					OnMaxHealthPointsChanged.Broadcast(Data.NewValue);
				}
			}
	);
}

void UAGASProgressBarWidgetController::BroadcastInitialValues()
{
	OnHealthPointsChanged.Broadcast(AGASAttributeSet->GetHealthPoints());
	OnMaxHealthPointsChanged.Broadcast(AGASAttributeSet->GetMaxHealthPoints());
}
