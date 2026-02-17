// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASCheckpoint.h"
#include "AGASMapEntrance.generated.h"

UCLASS()
class AURAGAS_API AAGASMapEntrance : public AAGASCheckpoint
{
	GENERATED_BODY()

public:
	AAGASMapEntrance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	//~ Being Highlight Interface
	virtual void HighlightActor_Implementation() override;
	//~ End Highlight Interface
	
	//~ Being Save Interface
	virtual void LoadActor_Implementation() override;
	//~ End Save Interface
	
	// Map we want to travel to
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;
	
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;
};
