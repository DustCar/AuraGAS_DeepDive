// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASDamageGameplayAbility.h"
#include "AGASGA_CastBeam.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASGA_CastBeam : public UAGASDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);
	
	UFUNCTION(BlueprintCallable)
	void StoreOwnerPlayerController();
	
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);
	
	UFUNCTION(BlueprintCallable)
	void RemoveOnDeathBindingFromPrimaryTarget();
	
	UFUNCTION(BlueprintCallable)
	void RemoveOnDeathBindingFromAdditionalTarget(AActor* AdditionalTarget);
	
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;
	
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;
	
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 MaxNumShockTargets = 5;
	
	// Determines how far to search for additional targets from the first
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	float AdditionalTargetRadius = 500.f;
	
};
