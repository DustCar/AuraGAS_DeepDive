// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AGASEnemySpawnPoint.generated.h"

class AAGASEnemy;

UCLASS()
class AURAGAS_API AAGASEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyClass")
	TSubclassOf<AAGASEnemy> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyClass")
	int32 EnemyLevel = 1;

protected:
	virtual void BeginPlay() override;
};
