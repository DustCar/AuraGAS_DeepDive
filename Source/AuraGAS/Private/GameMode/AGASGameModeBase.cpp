// Copyright Dustin Carmelo


#include "Game/AGASGameModeBase.h"

#include "Game/AGASLoadMenuSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_AGASLoadSlot.h"

void AAGASGameModeBase::SaveSlotData(UMVVM_AGASLoadSlot* LoadSlot)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, 0);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	UAGASLoadMenuSaveGame* LoadMenuSaveGame = Cast<UAGASLoadMenuSaveGame>(SaveGameObject);
	LoadMenuSaveGame->PlayerName = LoadSlot->PlayerName;
	
	UGameplayStatics::SaveGameToSlot(LoadMenuSaveGame, LoadSlot->LoadSlotName, 0);
}
