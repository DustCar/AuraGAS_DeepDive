// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASGameplayAbility.h"
#include "AGASGA_CastProjectile.generated.h"

class AAGASProjectile;
/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASGA_CastProjectile : public UAGASGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAGASProjectile> ProjectileClass;

private:

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
};
