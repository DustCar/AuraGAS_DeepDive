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

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage");
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
