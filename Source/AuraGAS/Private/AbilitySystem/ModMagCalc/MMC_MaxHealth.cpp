// Copyright Dustin Carmelo


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AGASAttributeSet.h"
#include "Interaction/AGASCombatInterface.h"
#include "Interaction/AGASModifierDependencyInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAGASAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Parameters needed to pass into a function that captures the attribute we are interested in
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	const int32 PlayerLevel = IAGASCombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	
	return BaseValue + AttributeMultiplier * Vigor + LevelMultiplier * PlayerLevel;
}

FOnExternalGameplayModifierDependencyChange* UMMC_MaxHealth::GetExternalModifierDependencyMulticast(
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
