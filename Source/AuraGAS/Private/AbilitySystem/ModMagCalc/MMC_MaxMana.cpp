// Copyright Dustin Carmelo


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AGASAttributeSet.h"
#include "Interaction/AGASCombatInterface.h"
// #include "Interaction/AGASModifierDependencyInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAGASAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather source and target tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	const int32 PlayerLevel = IAGASCombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	
	return BaseValue + AttributeMultiplier * Intelligence + LevelMultiplier * PlayerLevel;
}

// Only one of the MMCs needs to be overriding this function since the callback function OnCustomMagnitudeExternalDependencyFired
// that gets fired will iterate through all modifiers that rely on another attribute and recalculates them
// FOnExternalGameplayModifierDependencyChange* UMMC_MaxMana::GetExternalModifierDependencyMulticast(
// 	const FGameplayEffectSpec& Spec, UWorld* World) const
// {
// 	AActor* Instigator = Spec.GetContext().GetInstigator();
// 	IAGASModifierDependencyInterface* ModifierDependencyInterface = Cast<IAGASModifierDependencyInterface>(Instigator);
// 	if (ModifierDependencyInterface)
// 	{
// 		return ModifierDependencyInterface->GetOnModifierDependencyChanged();
// 	}
// 	return Super::GetExternalModifierDependencyMulticast(Spec, World);
// }
