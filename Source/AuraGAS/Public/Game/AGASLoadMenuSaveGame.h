// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "AGASLoadMenuSaveGame.generated.h"

class UGameplayAbility;

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()
	
	// used to identify our saved actor
	UPROPERTY()
	FName ActorName = FName();
	
	UPROPERTY()
	FTransform Transform = FTransform();

	// serialized variables from the Actor - only vars marked with SaveGame modifier
	UPROPERTY()
	TArray<uint8> Bytes;
};

// == overload for FSavedActor to allow the use of AddUnique if used in an array
inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()
	
	// identifier for map
	UPROPERTY()
	FString MapAssetName = FString();
	
	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatusTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityInputTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTypeTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel;
};

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
	
	UPROPERTY()
	bool bFirstTimeLoadIn = true;
	
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
	
	// Abilities
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;
	
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;
	
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
};
