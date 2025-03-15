// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
private:

	UPROPERTY(EditAnywhere, Category = "CalculationValues")
	float BaseValue = 1.f;

	UPROPERTY(EditAnywhere, Category = "CalculationValues")
	float AttributeMultiplier = 5.f;

	UPROPERTY(EditAnywhere, Category = "CalculationValues")
	float LevelMultiplier = 10.f;

	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
