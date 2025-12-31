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
	
public:
	
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);
	
	UFUNCTION(BlueprintCallable)
	void StoreOwnerPlayerController();
	
protected:
	
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;
	
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;
	
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
};
