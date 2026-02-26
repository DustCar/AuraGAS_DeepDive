// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AuraGAS/AuraGAS.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/AGASHighlightInterface.h"
#include "Interaction/AGASSaveInterface.h"
#include "AGASCheckpoint.generated.h"

class USphereComponent;

UCLASS()
class AURAGAS_API AAGASCheckpoint : public APlayerStart, public IAGASSaveInterface, public IAGASHighlightInterface
{
	GENERATED_BODY()

public:
	AAGASCheckpoint(const FObjectInitializer& ObjectInitializer);
	
	//~ Being Save Interface
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	//~ End Save Interface
	
	// a variable that we want to be serialized using the SaveGame specifier
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//~ Being Highlight Interface
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual FVector GetMoveToLocation() override;
	//~ End Highlight Interface
	
	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GlowFactor = 10.f;
	
	UPROPERTY(EditDefaultsOnly)
	bool bBindOverlapCallback = true;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShouldOcclude = false;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintCallable)
	void Glow();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
