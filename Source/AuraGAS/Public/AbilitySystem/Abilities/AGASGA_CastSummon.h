// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASGameplayAbility.h"
#include "AGASGA_CastSummon.generated.h"

class AAGASCharacterBase;
/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASGA_CastSummon : public UAGASGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();
	
	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<AAGASCharacterBase> GetRandomMinionClass();

	UFUNCTION(BlueprintCallable)
	void SpawnMinion(const FVector& SpawnLocation);
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<AAGASCharacterBase>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 125.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpreadAngle = 90.f;
};
