// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"

void UAGASDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FDamageEffectParams Params = MakeDamageEffectParamsFromClassDefaults(TargetActor);

	UAGASAbilitySystemLibrary::ApplyDamageEffectToTarget(Params);
}

int32 UAGASDamageGameplayAbility::GetRoundedDamageAtLevel(int32 Level)
{
	if (DamageFloat.IsValid())
	{
		return FMath::RoundHalfToEven(DamageFloat.GetValueAtLevel(Level));
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
		
		NamesToValues.Add(TEXT("_Dmg0"), DamageAbility->GetRoundedDamageAtLevel(Level));
		NamesToValues.Add(TEXT("_Dmg1"), DamageAbility->GetRoundedDamageAtLevel(Level + 1));
		NamesToValues.Add(TEXT("_LineBreak"), "\n");
		NamesToValues.Add(TEXT("_ProjNum0"), Level < 5 ? Level : 5);
		NamesToValues.Add(TEXT("_ProjNum1"), Level + 1 < 5 ? Level + 1 : 5);
		
		NewDescription = FString::Format(*Description, NamesToValues);
	}
	return NewDescription;
}

FDamageEffectParams UAGASDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.KnockbackGameplayEffectClass = KnockbackEffectClass;
	Params.KnockbackStatusGameplayEffectClass = KnockbackStatusEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = DamageFloat.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageTag;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DebuffDuration = DebuffDuration;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackChance = KnockbackChance;
	Params.KnockbackImpulseMagnitude = KnockbackImpulseMagnitude;
	
	// add impulses for melee characters
	if (IsValid(TargetActor))
	{
		FRotator ToTargetRotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		ToTargetRotation.Pitch = 45.f;
		const FVector ToTarget = ToTargetRotation.Vector();
		
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackDirection = ToTarget;
	}
	
	return Params;
}

