// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "AGASProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AURAGAS_API AAGASProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAGASProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	bool bHit = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	virtual void HandleSpecialEffectsOnImpact();
	
	bool IsValidOverlap(const AActor* OtherActor);

private:

	UPROPERTY(EditDefaultsOnly, Category = "CustomVariables")
	float LifeSpan = 10.f;

	UPROPERTY(EditAnywhere, Category = "CustomVariables|FX")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "CustomVariables|FX")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "CustomVariables|FX")
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
	UFUNCTION()
	void OnHomingTargetDeath(AActor* DeadActor);
	
};
