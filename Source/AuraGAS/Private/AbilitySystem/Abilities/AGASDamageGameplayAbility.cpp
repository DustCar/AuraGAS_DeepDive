// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

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
