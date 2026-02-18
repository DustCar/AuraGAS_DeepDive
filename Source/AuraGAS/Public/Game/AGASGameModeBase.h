// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AGASGameModeBase.generated.h"

class UAGASLoadMenuSaveGame;
class USaveGame;
class UMVVM_AGASLoadSlot;
/**
 * 
 */
UCLASS()
class AURAGAS_API AAGASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	void SaveSlotData(UMVVM_AGASLoadSlot* LoadSlot);
	
	UAGASLoadMenuSaveGame* GetSaveSlotData(const FString& SlotName) const;
	static void DeleteSlot(const FString& SlotName);
	UAGASLoadMenuSaveGame* RetrieveInGameSaveData();
	void SaveInGameProgressData(UAGASLoadMenuSaveGame* SaveObject);
	
	void SaveWorldState(UWorld* InWorld, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* InWorld) const;
	
	void TravelToMap(UMVVM_AGASLoadSlot* LoadSlot);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadMenuSaveGameClass;
	
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;
	
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
	
	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	void PlayerDied(ACharacter* DeadCharacter);
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
};
