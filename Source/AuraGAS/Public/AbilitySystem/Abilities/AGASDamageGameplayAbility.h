// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASGameplayAbility.h"
#include "AGASDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASDamageGameplayAbility : public UAGASGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage");
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
