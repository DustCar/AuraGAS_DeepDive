// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AGASLoadMenuSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASLoadMenuSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	FString PlayerName = FString("DefaultName");
	
	UPROPERTY()
	FString MapName = FString("DefaultMap");
};
