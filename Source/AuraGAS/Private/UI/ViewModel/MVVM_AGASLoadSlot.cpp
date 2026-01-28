// Copyright Dustin Carmelo


#include "UI/ViewModel/MVVM_AGASLoadSlot.h"

void UMVVM_AGASLoadSlot::InitializeSlot()
{
	// TODO: Set slot status based on loaded data. 
	SetWidgetSwitcherIndex.Broadcast(2);
}
