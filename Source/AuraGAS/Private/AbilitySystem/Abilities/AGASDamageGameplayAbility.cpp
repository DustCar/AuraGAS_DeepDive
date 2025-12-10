// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AGASGameplayTags.h"

void UAGASDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);

	for (const auto& [DamageTag, DamageScaleFloat] : DamageTypes)
	{
		const float ScaledDamage = DamageScaleFloat.GetValueAtLevel(GetAbilityLevel());
		DamageSpecHandle.Data->SetSetByCallerMagnitude(DamageTag, ScaledDamage);
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

int32 UAGASDamageGameplayAbility::GetRoundedDamageAtLevel(int32 Level, const FGameplayTag& DamageTag)
{
	if (DamageTypes.Find(DamageTag))
	{
		return FMath::RoundHalfToEven(DamageTypes.Find(DamageTag)->GetValueAtLevel(Level));
	}
	
	return 0;
}

FString UAGASDamageGameplayAbility::FormatDamageAbilityDescription(int32 Level, const FString& Description,
	UGameplayAbility* Ability)
{
	FString NewDescription = Description;
	
	if (UAGASDamageGameplayAbility* DamageAbility = Cast<UAGASDamageGameplayAbility>(Ability))
	{
		FStringFormatNamedArguments NamesToValues;
		
		NamesToValues.Add(TEXT("_FireDmg0"), DamageAbility->GetRoundedDamageAtLevel(Level, TAG_Damage_Fire));
		NamesToValues.Add(TEXT("_FireDmg1"), DamageAbility->GetRoundedDamageAtLevel(Level + 1, TAG_Damage_Fire));
		NamesToValues.Add(TEXT("_LightningDmg0"), DamageAbility->GetRoundedDamageAtLevel(Level, TAG_Damage_Lightning));
		NamesToValues.Add(TEXT("_LightningDmg1"), DamageAbility->GetRoundedDamageAtLevel(Level + 1, TAG_Damage_Lightning));
		NamesToValues.Add(TEXT("_ArcaneDmg0"), DamageAbility->GetRoundedDamageAtLevel(Level, TAG_Damage_Arcane));
		NamesToValues.Add(TEXT("_ArcaneDmg1"), DamageAbility->GetRoundedDamageAtLevel(Level + 1, TAG_Damage_Arcane));
		NamesToValues.Add(TEXT("_PhysicalDmg0"), DamageAbility->GetRoundedDamageAtLevel(Level, TAG_Damage_Physical));
		NamesToValues.Add(TEXT("_PhysicalDmg1"), DamageAbility->GetRoundedDamageAtLevel(Level + 1, TAG_Damage_Physical));
		NamesToValues.Add(TEXT("_LineBreak"), "\n");
		NamesToValues.Add(TEXT("_ProjNum0"), Level < 5 ? Level : 5);
		NamesToValues.Add(TEXT("_ProjNum1"), Level + 1 < 5 ? Level + 1 : 5);
		
		NewDescription = FString::Format(*Description, NamesToValues);
	}
	return NewDescription;
}

