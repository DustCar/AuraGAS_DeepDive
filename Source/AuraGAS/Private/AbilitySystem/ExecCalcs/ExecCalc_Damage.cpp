// Copyright Dustin Carmelo


#include "AbilitySystem/ExecCalcs/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AGASAbilityTypes.h"
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
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)

	// Damage resistances
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

	// Map to pair ResistanceTag to ResistanceGECaptureDef to be used in Execute()
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AGASDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, CriticalHitResistance, Target, false);

		// Damage resistances
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAGASAttributeSet, PhysicalResistance, Target, false);

		// Add pairings to TagsToCaptureDefs
		TagsToCaptureDefs.Add(TAG_Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(TAG_Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(TAG_Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(TAG_Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(TAG_Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(TAG_Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		
		TagsToCaptureDefs.Add(TAG_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(TAG_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(TAG_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(TAG_Resistance_Physical, PhysicalResistanceDef);
	}
	
};

static const AGASDamageStatics& DamageStatics()
{
	static AGASDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// Add capture defs to RelevantAttributeToCapture
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);

	AllDamageTypes = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Damage);
	AllResistanceTypes = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Resistance);

	// Add DamageType to Resistance tag pairings to DamageToResistMap
	for (int32 i = 0; i < AllDamageTypes.Num(); i++)
	{
		DamageToResistMap.Add(AllDamageTypes.GetByIndex(i), AllResistanceTypes.GetByIndex(i));
	}
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

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Loop through all the Damage Types and add it to Damage
	float Damage = 0.f;
	for (const auto& [DamageTypeTag, ResistanceTypeTag] : DamageToResistMap)
	{
		// Get DamageTypeValue by set by caller magnitude using its tag
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);

		// check if resistance tag is available in TagsToCaptureDefs then save that capture def
		checkf(DamageStatics().TagsToCaptureDefs.Contains(ResistanceTypeTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTypeTag.ToString())
		const FGameplayEffectAttributeCaptureDefinition ResCaptureDef = DamageStatics().TagsToCaptureDefs[ResistanceTypeTag];

		// Get value of captured resistance attribute
		float ResistanceTypeValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResCaptureDef, EvaluateParams, ResistanceTypeValue);
		ResistanceTypeValue = FMath::Clamp(ResistanceTypeValue, 0.f, 100.f);

		// Reduce current damage value by each percent of damage resist the target has
		DamageTypeValue *= (100.f - ResistanceTypeValue) / 100.f;
		
		Damage += DamageTypeValue;
	}

	// Critical Hits ignore block and armor, and do double damage plus Crit Damage
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParams, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Clamp(SourceCriticalHitChance, 0.f, 100.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParams, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(0.f, SourceCriticalHitDamage);

	// Critical Resist reduces the chance of Critical Hits happening
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParams, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Clamp(TargetCriticalHitResistance, 0.f, 100.f);

	const FRealCurve* CritResistCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CritResistCoefficient = CritResistCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	
	float EffectiveCriticalHitChance = SourceCriticalHitChance * ((100 - TargetCriticalHitResistance * CritResistCoefficient) / 100.f);
	const bool bCriticalHit = FMath::FRandRange(UE_SMALL_NUMBER, 100.f) < EffectiveCriticalHitChance;
	UAGASAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	// Determine if a critical is hit and if so then ignore block chance and damage resist
	if (bCriticalHit)
	{
		Damage = Damage * 2 + SourceCriticalHitDamage;
	}
	else
	{
		// Capture BlockChance on Target, and determine if there was a successful block
		// If block, half the damage
		float TargetBlockChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParams, TargetBlockChance);
		TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

		const bool bBlocked = FMath::FRandRange(UE_SMALL_NUMBER, 100.f) < TargetBlockChance;
		UAGASAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
		if (bBlocked)
		{
			Damage *= 0.5;
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
	}

	const FGameplayModifierEvaluatedData EvaluatedData(UAGASAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
