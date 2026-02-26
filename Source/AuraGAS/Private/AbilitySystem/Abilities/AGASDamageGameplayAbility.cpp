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

int32 UAGASDamageGameplayAbility::GetRoundedDamageAtLevel(int32 Level) const
{
	if (DamageFloat.IsValid())
	{
		return FMath::RoundFromZero(DamageFloat.GetValueAtLevel(Level));
	}
	
	return 0;
}

FString UAGASDamageGameplayAbility::FormatDamageAbilityDescription(int32 Level, const FString& Description,
	UGameplayAbility* Ability)
{
	FString NewDescription = Description;
	FStringFormatNamedArguments NamesToValues;
	
	NamesToValues.Add(TEXT("_Dmg0"), GetRoundedDamageAtLevel(Level));
	NamesToValues.Add(TEXT("_Dmg1"), GetRoundedDamageAtLevel(Level + 1));
	NamesToValues.Add(TEXT("_LineBreak"), "\n");
	
	NewDescription = FString::Format(*Description, NamesToValues);
	
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
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackChance = KnockbackChance;
	Params.KnockbackImpulseMagnitude = KnockbackImpulseMagnitude;
	
	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageOrigin = RadialDamageOrigin;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	
	// add impulses for other abilities with a valid target actor
	if (IsValid(TargetActor))
	{
		FRotator ToTargetRotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		
		ToTargetRotation.Pitch = 45.f;
		FVector ToTarget = ToTargetRotation.Vector();
		ToTarget.Normalize();
		
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackDirection = ToTarget;
	}
	
	return Params;
}

