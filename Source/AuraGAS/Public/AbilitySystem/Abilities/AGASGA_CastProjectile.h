// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASDamageGameplayAbility.h"
#include "AGASGA_CastProjectile.generated.h"

class AAGASProjectile;
/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASGA_CastProjectile : public UAGASDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAGASProjectile> ProjectileClass;
	
};
