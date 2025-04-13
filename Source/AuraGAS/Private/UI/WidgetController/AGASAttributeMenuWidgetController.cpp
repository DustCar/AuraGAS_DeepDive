// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASAttributeMenuWidgetController.h"

#include "AGASGameplayTags.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "AbilitySystem/Data/AGASAttributeInfo.h"

void UAGASAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	FAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(TAG_Attributes_Primary_Strength);
	Info.AttributeValue = AGASAttributeSet->GetStrength();

	AttributeInfoDelegate.Broadcast(Info);
}

void UAGASAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}
