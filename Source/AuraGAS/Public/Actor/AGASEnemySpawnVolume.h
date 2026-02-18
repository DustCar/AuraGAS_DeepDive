// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/AGASSaveInterface.h"
#include "AGASEnemySpawnVolume.generated.h"

class AAGASEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class AURAGAS_API AAGASEnemySpawnVolume : public AActor, public IAGASSaveInterface
{
	GENERATED_BODY()

public:
	AAGASEnemySpawnVolume();
	
	//~ Begin Save Interface
	virtual void LoadActor_Implementation() override;
	//~ End Save Interface
	
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
	TArray<AAGASEnemySpawnPoint*> SpawnPoints;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
};
