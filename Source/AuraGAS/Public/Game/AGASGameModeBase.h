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
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadMenuSaveGameClass;
};
