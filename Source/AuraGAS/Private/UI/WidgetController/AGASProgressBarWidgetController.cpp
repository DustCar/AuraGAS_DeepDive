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
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AGASAttributeSet->GetMaxHealthPointsAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
			{
				if (Data.NewValue == 0 && bFirstBroadcast)
				{
					OnMaxHealthPointsChanged.Broadcast(AGASAttributeSet->GetMaxHealthPoints());
				}
				else
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
