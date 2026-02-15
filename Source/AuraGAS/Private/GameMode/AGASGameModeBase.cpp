// Copyright Dustin Carmelo


#include "Game/AGASGameModeBase.h"

#include "EngineUtils.h"
#include "AuraGAS/AGASLogChannels.h"
#include "Game/AGASGameInstance.h"
#include "Game/AGASLoadMenuSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/AGASSaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_AGASLoadSlot.h"

void AAGASGameModeBase::SaveSlotData(UMVVM_AGASLoadSlot* LoadSlot)
{
	DeleteSlot(LoadSlot->GetLoadSlotName());
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	UAGASLoadMenuSaveGame* LoadMenuSaveGame = Cast<UAGASLoadMenuSaveGame>(SaveGameObject);
	LoadMenuSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadMenuSaveGame->MapName = LoadSlot->GetMapName();
	LoadMenuSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadMenuSaveGame->PlayerLevel = LoadSlot->GetPlayerLevel();
	
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

void AAGASGameModeBase::SaveWorldState(UWorld* InWorld) const
{
	FString WorldName = InWorld->GetMapName();
	// Map names have a prefix that is prepended when obtaining it from world; we need to remove them to get true map name
	WorldName.RemoveFromStart(InWorld->StreamingLevelsPrefix);
	
	UAGASGameInstance* AGASGameInstance = Cast<UAGASGameInstance>(GetGameInstance());
	check(AGASGameInstance);
	
	if (UAGASLoadMenuSaveGame* SaveGame = GetSaveSlotData(AGASGameInstance->LoadSlotName))
	{
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}
		
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		// clear out saved "actors" to refill with new ones
		SavedMap.SavedActors.Empty();
		
		// looping through literally all actors in the world, skipping invalid actors
		for (FActorIterator It(InWorld); It; ++It)
		{
			AActor* Actor = *It;
			
			if (!IsValid(Actor) || !Actor->Implements<UAGASSaveInterface>()) continue;
			
			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();
			
			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			
			Actor->Serialize(Archive);
			
			SavedMap.SavedActors.AddUnique(SavedActor);
		}
		
		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		
		UGameplayStatics::SaveGameToSlot(SaveGame, AGASGameInstance->LoadSlotName, 0);
	}
}

void AAGASGameModeBase::LoadWorldState(UWorld* InWorld) const
{
	FString WorldName = InWorld->GetMapName();
	WorldName.RemoveFromStart(InWorld->StreamingLevelsPrefix);
	
	UAGASGameInstance* AGASGameInstance = Cast<UAGASGameInstance>(GetGameInstance());
	check(AGASGameInstance);
	
	if (UGameplayStatics::DoesSaveGameExist(AGASGameInstance->LoadSlotName, 0))
	{
		UAGASLoadMenuSaveGame* SaveGame = Cast<UAGASLoadMenuSaveGame>(UGameplayStatics::LoadGameFromSlot(AGASGameInstance->LoadSlotName, 0));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogAGAS, Error, TEXT("Failed to load slot."))
			return;
		}
		
		for (FActorIterator It(InWorld); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<UAGASSaveInterface>()) continue;
		
			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (IAGASSaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}
					
					FMemoryReader MemoryReader(SavedActor.Bytes);
					
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					// converts bytes back into variables
					Actor->Serialize(Archive);
					
					IAGASSaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
	
	
	
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
