// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/Data/AGASCharacterClassInfo.h"
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
	
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
						 const FGameplayEffectSpec& Spec,
						 const FAggregatorEvaluateParameters& EvaluateParams,
						 FGameplayEffectContextHandle& EffectContextHandle) const;
	float CalculateBaseDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
							  const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluateParams) const;
	float CalculatePercentDamageIgnored(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
										AActor* SourceAvatar, AActor* TargetAvatar,
										const UAGASCharacterClassInfo* CharacterClassInfo,
										const FAggregatorEvaluateParameters& EvaluateParams) const;

	UPROPERTY()
	TMap<FGameplayTag, FGameplayTag> DamageToResistMap;
	
	UPROPERTY()
	TMap<FGameplayTag, FGameplayTag> DamageToDebuffMap;
};
