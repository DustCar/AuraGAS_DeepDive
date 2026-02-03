// Copyright Dustin Carmelo


#include "UI/ViewModel/MVVM_AGASLoadSlot.h"

void UMVVM_AGASLoadSlot::SetLoadSlotWidget(const ELoadSlotWidget LoadSlotWidget)
{
	SetWidgetSwitcherIndex.Broadcast(LoadSlotWidget);
}

void UMVVM_AGASLoadSlot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_AGASLoadSlot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

void UMVVM_AGASLoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}
