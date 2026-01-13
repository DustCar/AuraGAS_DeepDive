// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGASMagicCircle.generated.h"

class USphereComponent;

UCLASS()
class AURAGAS_API AAGASMagicCircle : public AActor
{
	GENERATED_BODY()

public:
	AAGASMagicCircle();
	
	virtual void Tick(float DeltaTime) override;
	
	void SetSphereRadius(float Radius);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnMagicCircleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnMagicCircleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> MagicCircleSphere;
};
