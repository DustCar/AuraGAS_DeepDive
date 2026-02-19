// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AGASLootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawning")
	TSubclassOf<AActor> LootClass;
	
	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	float DropChance = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	int32 MaxDropNumber = 0;
};

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASLootTiers : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();
	
	UPROPERTY(EditDefaultsOnly, Category = "LootTiers|Spawning")
	TArray<FLootItem> LootItems;
};
