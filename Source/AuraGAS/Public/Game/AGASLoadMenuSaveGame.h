// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AGASLoadMenuSaveGame.generated.h"

/**
 *	Realistically, this is the class that holds everything we want to save for Aura and the levels
 */
UCLASS()
class AURAGAS_API UAGASLoadMenuSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	FString PlayerName = FString("DefaultName");
	
	UPROPERTY()
	FString MapName = FString("DefaultMapName");
	
	UPROPERTY()
	FName PlayerStartTag;
	
	/* Player */
	UPROPERTY()
	int32 PlayerLevel = 0;
	
	UPROPERTY()
	int32 XPPoints = 0;
	
	UPROPERTY()
	int32 SpellPoints = 0;
	
	UPROPERTY()
	int32 AttributePoints = 0;
	
	//~ Primary Attributes Begin
	UPROPERTY()
	float Strength = 0;
	
	UPROPERTY()
	float Intelligence = 0;
	
	UPROPERTY()
	float Resilience = 0;
	
	UPROPERTY()
	float Vigor = 0;
	//~ Primary Attributes End
};
