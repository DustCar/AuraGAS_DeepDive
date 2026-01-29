// Copyright Dustin Carmelo


#include "Game/AGASGameModeBase.h"

#include "Game/AGASLoadMenuSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_AGASLoadSlot.h"

void AAGASGameModeBase::SaveSlotData(UMVVM_AGASLoadSlot* LoadSlot)
{
	DeleteSlot(LoadSlot->LoadSlotName);
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	UAGASLoadMenuSaveGame* LoadMenuSaveGame = Cast<UAGASLoadMenuSaveGame>(SaveGameObject);
	LoadMenuSaveGame->PlayerName = LoadSlot->PlayerName;
	
	UGameplayStatics::SaveGameToSlot(LoadMenuSaveGame, LoadSlot->LoadSlotName, 0);
}

UAGASLoadMenuSaveGame* AAGASGameModeBase::GetSaveSlotData(const FString& SlotName) const
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		return Cast<UAGASLoadMenuSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	}
	return nullptr;
}

void AAGASGameModeBase::DeleteSlot(const FString& SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, 0);
	}
}
