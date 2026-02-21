// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_SimpleAttributeBased.generated.h"

/**
 * An MMC class that calculates a value using simple operations based off of one, or more, attributes and their multipliers
 * Ex: BaseMultiplier + (Attr1 * Attr1Coeff + Attr2 * Attr2Coeff + ...) + LevelMultiplier * PlayerLevel
 * 
 * The parenthesis encapsulates the portion that this class would provide in BPs. This is not the class to use for more
 * complex operations. Just one that allows for quick and easy values based off of attributes
 * 
 * For RelevantAttributesToCapture, set that within the BP since it is exposed by default. That's where you will be adding
 * the attributes wanted
 */
UCLASS()
class AURAGAS_API UMMC_SimpleAttributeBased : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
	// needed so that MMCs get recalculated when an attribute changes from an external source, not from the ability system
	virtual FOnExternalGameplayModifierDependencyChange* GetExternalModifierDependencyMulticast(const FGameplayEffectSpec& Spec, UWorld* World) const override;
	
private:

	UPROPERTY(EditAnywhere, Category = "CalculationValues")
	float BaseValue = 1.f;

	UPROPERTY(EditAnywhere, Category = "CalculationValues")
	float LevelMultiplier = 10.f;
	
	// make sure that the indices match for the attributes you want
	// i.e. if you have attributes {Strength, Intelligence}, and you want STR * 2 and INT * 3, then AttrCoeffs = {2, 3}
	UPROPERTY(EditDefaultsOnly, Category = "CalculationValues")
	TArray<float> AttributeCoefficients;
};
