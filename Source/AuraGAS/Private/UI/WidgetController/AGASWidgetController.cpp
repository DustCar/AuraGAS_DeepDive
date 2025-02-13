// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASWidgetController.h"

void UAGASWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AGASAttributeSet = WCParams.AGASAttributeSet;
}

void UAGASWidgetController::BroadcastInitialValues()
{
	
}

void UAGASWidgetController::BindCallbacksToDependencies()
{
	
}
