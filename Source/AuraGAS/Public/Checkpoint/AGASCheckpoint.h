// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/AGASSaveInterface.h"
#include "AGASCheckpoint.generated.h"

class USphereComponent;

UCLASS()
class AURAGAS_API AAGASCheckpoint : public APlayerStart, public IAGASSaveInterface
{
	GENERATED_BODY()

public:
	AAGASCheckpoint(const FObjectInitializer& ObjectInitializer);
	
	//~ Being Save Interface
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	//~ End Save Interface
	
	// a variable that we want to be serialized using the SaveGame specifier
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	void Glow();
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
