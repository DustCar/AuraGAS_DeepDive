// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASProjectile.h"
#include "AGASFireBall.generated.h"

UCLASS()
class AURAGAS_API AAGASFireBall : public AAGASProjectile
{
	GENERATED_BODY()

public:
	AAGASFireBall();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	// an array that will keep track of enemies that we have already passed through;
	// avoids overlapping/damagine an actor multiple times in a short interval
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> IgnoreList;
	
	UFUNCTION(BlueprintCallable)
	void EmptyIgnoreList();
	
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandleSpecialEffectsOnImpact() override;
	
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	float RadialDamageInnerRadius = 50.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	float RadialDamageOuterRadius = 160.f;
};
