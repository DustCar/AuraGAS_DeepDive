// Copyright Dustin Carmelo


#include "AbilitySystem/ExecCalcs/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AGASGameplayTags.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "AbilitySystem/Data/AGASCharacterClassInfo.h"
#include "Interaction/AGASCombatInterface.h"

/* Raw C++ struct that will not be exposed to the reflection system */
struct AGASDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	
	AGASDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, BlockChance, Target, false);
	}
	
};

static const AGASDamageStatics& DamageStatics()
{
	static AGASDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Setup important vars that could be used
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	IAGASCombatInterface* SourceCombatInterface = Cast<IAGASCombatInterface>(SourceAvatar);
	IAGASCombatInterface* TargetCombatInterface = Cast<IAGASCombatInterface>(TargetAvatar);

	const UAGASCharacterClassInfo* CharacterClassInfo = UAGASAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	// Get Damage set by caller magnitude
	float Damage = Spec.GetSetByCallerMagnitude(TAG_Damage);

	// Capture BlockChance on Target, and determine if there was a successful block
	// If block, half the damage
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParams, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

	// TODO: Implement Critical Hits

	const bool bBlocked = FMath::FRandRange(UE_SMALL_NUMBER, 100.f) < TargetBlockChance;
	if (bBlocked)
	{
		Damage *= 0.5;
		UE_LOG(LogTemp, Warning, TEXT("Attack Blocked!"))
	}

	// Calculate Armor
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParams, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	const FRealCurve* ArmorPenCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenCoefficient = ArmorPenCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	// Calculate ArmorPen
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParams, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Clamp(SourceArmorPenetration, 0.f, 100.f);

	// ArmorPenetration ignores a percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor * ((100 - SourceArmorPenetration * ArmorPenCoefficient) / 100.f);

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	// Armor ignores a percentage of Damage
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	const FGameplayModifierEvaluatedData EvaluatedData(UAGASAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
