// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:

	UPROPERTY()
	FGameplayTagContainer AllDamageTypes;

	UPROPERTY()
	FGameplayTagContainer AllResistanceTypes;

	UPROPERTY()
	TMap<FGameplayTag, FGameplayTag> DamageToResistMap;
};
