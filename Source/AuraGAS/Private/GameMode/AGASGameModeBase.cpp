// Copyright Dustin Carmelo


#include "Game/AGASGameModeBase.h"

#include "Game/AGASLoadMenuSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_AGASLoadSlot.h"

void AAGASGameModeBase::SaveSlotData(UMVVM_AGASLoadSlot* LoadSlot)
{
	DeleteSlot(LoadSlot->GetLoadSlotName());
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	UAGASLoadMenuSaveGame* LoadMenuSaveGame = Cast<UAGASLoadMenuSaveGame>(SaveGameObject);
	LoadMenuSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadMenuSaveGame->MapName = LoadSlot->GetMapName();
	
	UGameplayStatics::SaveGameToSlot(LoadMenuSaveGame, LoadSlot->GetLoadSlotName(), 0);
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

void AAGASGameModeBase::TravelToMap(UMVVM_AGASLoadSlot* LoadSlot)
{
	const TSoftObjectPtr<UWorld> Map = Maps.FindChecked(LoadSlot->GetMapName());
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, Map);
}

void AAGASGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	Maps.Add(DefaultMapName, DefaultMap);
}
