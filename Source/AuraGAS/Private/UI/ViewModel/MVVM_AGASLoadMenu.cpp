// Copyright Dustin Carmelo


#include "UI/ViewModel/MVVM_AGASLoadMenu.h"

#include "Game/AGASGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_AGASLoadSlot.h"

void UMVVM_AGASLoadMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_AGASLoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlots.Add(0, LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_AGASLoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlots.Add(1, LoadSlot_1);
	
	LoadSlot_2 = NewObject<UMVVM_AGASLoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_AGASLoadSlot* UMVVM_AGASLoadMenu::GetLoadSlotViewModelByIndex(int32 InIndex) const
{
	return LoadSlots.FindChecked(InIndex);
}

void UMVVM_AGASLoadMenu::NewSaveButtonPressed(int32 InSlot, const FString EnteredName)
{
	AAGASGameModeBase* AGASGameMode = Cast<AAGASGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	if (AGASGameMode)
	{
		LoadSlots[InSlot]->PlayerName = EnteredName;
		AGASGameMode->SaveSlotData(LoadSlots[InSlot]);
		
		LoadSlots[InSlot]->InitializeSlot();
	}
}

void UMVVM_AGASLoadMenu::NewGameButtonPressed(int32 InSlot)
{
	LoadSlots[InSlot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_AGASLoadMenu::LoadSaveButtonPressed(int32 InSlot)
{
}
