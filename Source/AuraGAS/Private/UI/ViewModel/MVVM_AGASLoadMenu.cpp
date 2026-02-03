// Copyright Dustin Carmelo


#include "UI/ViewModel/MVVM_AGASLoadMenu.h"

#include "Game/AGASGameModeBase.h"
#include "Game/AGASLoadMenuSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_AGASLoadSlot.h"

void UMVVM_AGASLoadMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_AGASLoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlots.Add(0, LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_AGASLoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlots.Add(1, LoadSlot_1);
	
	LoadSlot_2 = NewObject<UMVVM_AGASLoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlots.Add(2, LoadSlot_2);
	
	SetNumLoadSlots(LoadSlots.Num());
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
		LoadSlots[InSlot]->SetPlayerName(EnteredName);
		LoadSlots[InSlot]->SetMapName(AGASGameMode->DefaultMapName);
		AGASGameMode->SaveSlotData(LoadSlots[InSlot]);
		LoadSlots[InSlot]->SetLoadSlotWidget(ELoadSlotWidget::Taken);
	}
}

void UMVVM_AGASLoadMenu::NewGameButtonPressed(int32 InSlot)
{
	LoadSlots[InSlot]->SetWidgetSwitcherIndex.Broadcast(ELoadSlotWidget::EnterName);
}

void UMVVM_AGASLoadMenu::SelectSaveButtonPressed(int32 InSlot)
{
	SaveSelected.Broadcast();
	for (const auto [SlotIndex, Slot] : LoadSlots)
	{
		if (SlotIndex == InSlot)
		{
			Slot->EnableSelectSaveButton.Broadcast(false);
		}
		else
		{
			Slot->EnableSelectSaveButton.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[InSlot];
}

void UMVVM_AGASLoadMenu::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAGASGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName());
		SelectedSlot->SetLoadSlotWidget(ELoadSlotWidget::Vacant);
		SelectedSlot->EnableSelectSaveButton.Broadcast(true);
	}
}

void UMVVM_AGASLoadMenu::PlayButtonPressed()
{
	AAGASGameModeBase* AGASGameMode = Cast<AAGASGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	if (AGASGameMode && IsValid(SelectedSlot))
	{
		AGASGameMode->TravelToMap(SelectedSlot);
	}
}

void UMVVM_AGASLoadMenu::LoadData()
{
	AAGASGameModeBase* AGASGameMode = Cast<AAGASGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	for (const auto [SlotIndex, Slot] : LoadSlots)
	{
		UAGASLoadMenuSaveGame* SaveObject = AGASGameMode->GetSaveSlotData(Slot->GetLoadSlotName());
		if (SaveObject)
		{
			Slot->SetPlayerName(SaveObject->PlayerName);
			Slot->SetMapName(SaveObject->MapName);
			Slot->SetLoadSlotWidget(ELoadSlotWidget::Taken);
		}
	}
}

void UMVVM_AGASLoadMenu::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
