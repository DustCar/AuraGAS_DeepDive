// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASAttributeMenuWidgetController.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "AbilitySystem/Data/AGASAttributeInfo.h"

void UAGASAttributeMenuWidgetController::BroadcastAttributeInfo(const FAttributeInfo& Info) const
{
	FAttributeInfo NewInfo = Info;
	NewInfo.AttributeValue = Info.AttributeToGet.GetNumericValue(AGASAttributeSet);
	AttributeInfoDelegate.Broadcast(NewInfo);
}

void UAGASAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	for (const auto& Info : AttributeInfo->AttributeInformation)
	{
		BroadcastAttributeInfo(Info);
	}
}

void UAGASAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);

	// loops through all attributes included in the AttributeInfo Data Asset and binds a lambda function to the attribute
	// value change delegate which updates the attribute menu values by calling BroadcastAttributeInfo.
	for (const auto& Info : AttributeInfo->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Info.AttributeToGet).AddLambda(
			[this, &Info] (const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Info);
			}
		);
	}
}
