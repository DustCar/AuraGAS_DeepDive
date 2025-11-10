// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	// after some testing, it seems that only one of the MMCs need to override this function to recalculate other MMCs
	virtual FOnExternalGameplayModifierDependencyChange* GetExternalModifierDependencyMulticast(const FGameplayEffectSpec& Spec, UWorld* World) const override;

private:

	UPROPERTY(EditAnywhere, Category = "CalculationValues")
	float BaseValue = 1.f;

	UPROPERTY(EditAnywhere, Category = "CalculationValues")
	float AttributeMultiplier = 5.f;

	UPROPERTY(EditAnywhere, Category = "CalculationValues")
	float LevelMultiplier = 10.f;

	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
