// Copyright Dustin Carmelo


#include "AbilitySystem/ModMagCalc/MMC_SimpleAttributeBased.h"

#include "Interaction/AGASCombatInterface.h"
#include "Interaction/AGASModifierDependencyInterface.h"

float UMMC_SimpleAttributeBased::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Parameters needed to pass into a function that captures the attribute we are interested in
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	int32 CurrentAttributeIndex = 0;
	float TotalAttributeAdditions = 0.f;
	for (FGameplayEffectAttributeCaptureDefinition CapturedAttribute : RelevantAttributesToCapture)
	{
		float AttributeValue = 0.f;
		GetCapturedAttributeMagnitude(CapturedAttribute, Spec, EvaluationParameters, AttributeValue);
		// want to avoid negatives from the attribute magnitude
		AttributeValue = FMath::Max<float>(AttributeValue, 0.f);
		
		TotalAttributeAdditions += (AttributeValue * AttributeCoefficients[CurrentAttributeIndex]);
		
		CurrentAttributeIndex++;
	}

	const int32 PlayerLevel = IAGASCombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	
	return BaseValue + TotalAttributeAdditions + LevelMultiplier * PlayerLevel;
}

FOnExternalGameplayModifierDependencyChange* UMMC_SimpleAttributeBased::GetExternalModifierDependencyMulticast(
	const FGameplayEffectSpec& Spec, UWorld* World) const
{
	AActor* Instigator = Spec.GetContext().GetInstigator();
	IAGASModifierDependencyInterface* ModifierDependencyInterface = Cast<IAGASModifierDependencyInterface>(Instigator);
	if (ModifierDependencyInterface)
	{
		return ModifierDependencyInterface->GetOnModifierDependencyChanged();
	}
	return Super::GetExternalModifierDependencyMulticast(Spec, World);
}
