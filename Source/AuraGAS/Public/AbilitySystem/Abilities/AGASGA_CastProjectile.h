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
	
public:
	virtual FString FormatDamageAbilityDescription(int32 Level, const FString& Description, UGameplayAbility* Ability) override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAGASProjectile> ProjectileClass;
	
	// spawns a single projectile
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag);
	
	// spawns multiple projectiles (1 - 5)
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectilesFromSocket(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget, const bool bOverridePitch, const float PitchAmount, const bool bHoming);
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	TArray<AAGASProjectile*> SpawnProjectilesFromCharacter();
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Params")
	float ProjectileSpread = 90.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Params")
	int32 MaxNumProjectiles = 5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Params")
	float HomingAccelerationMin = 1600.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Params")
	float HomingAccelerationMax = 3000.f;
};
