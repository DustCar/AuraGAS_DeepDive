// Copyright Dustin Carmelo


#include "Game/AGASGameModeBase.h"

#include "Game/AGASGameInstance.h"
#include "Game/AGASLoadMenuSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_AGASLoadSlot.h"

void AAGASGameModeBase::SaveSlotData(UMVVM_AGASLoadSlot* LoadSlot)
{
	DeleteSlot(LoadSlot->GetLoadSlotName());
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	UAGASLoadMenuSaveGame* LoadMenuSaveGame = Cast<UAGASLoadMenuSaveGame>(SaveGameObject);
	LoadMenuSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadMenuSaveGame->MapName = LoadSlot->GetMapName();
	LoadMenuSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	
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

UAGASLoadMenuSaveGame* AAGASGameModeBase::RetrieveInGameSaveData()
{
	UAGASGameInstance* AGASGameInstance = Cast<UAGASGameInstance>(GetGameInstance());
	
	const FString InGameLoadSlotName = AGASGameInstance->LoadSlotName;
	
	return GetSaveSlotData(InGameLoadSlotName);
}

void AAGASGameModeBase::SaveInGameProgressData(UAGASLoadMenuSaveGame* SaveObject)
{
	UAGASGameInstance* AGASGameInstance = Cast<UAGASGameInstance>(GetGameInstance());
	
	const FString InGameLoadSlotName = AGASGameInstance->LoadSlotName;
	AGASGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;
	
	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, 0);
}

void AAGASGameModeBase::TravelToMap(UMVVM_AGASLoadSlot* LoadSlot)
{
	const TSoftObjectPtr<UWorld> Map = Maps.FindChecked(LoadSlot->GetMapName());
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, Map);
}

AActor* AAGASGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAGASGameInstance* AGASGameInstance = Cast<UAGASGameInstance>(GetGameInstance());
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
			if (PlayerStart->PlayerStartTag == AGASGameInstance->PlayerStartTag)
			{
				SelectedActor = PlayerStart;
				break;
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void AAGASGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	Maps.Add(DefaultMapName, DefaultMap);
}
