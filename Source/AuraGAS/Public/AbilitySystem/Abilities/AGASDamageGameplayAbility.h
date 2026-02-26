// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASAbilityTypes.h"
#include "AGASGameplayAbility.h"
#include "AGASDamageGameplayAbility.generated.h"

// FStruct FStringFormatNamedArguments;

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASDamageGameplayAbility : public UAGASGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
	int32 GetRoundedDamageAtLevel(int32 Level) const;
	
	virtual FString FormatDamageAbilityDescription(int32 Level, const FString& Description, UGameplayAbility* Ability);
	
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Knockback")
	TSubclassOf<UGameplayEffect> KnockbackEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Knockback")
	TSubclassOf<UGameplayEffect> KnockbackStatusEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (Categories = "Damage"));
	FGameplayTag DamageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage");
	FScalableFloat DamageFloat;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff");
	float DebuffChance = 20.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff");
	float DebuffDamage = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|DeathImpulse")
	float DeathImpulseMagnitude = 2500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Knockback")
	float KnockbackChance = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Knockback")
	float KnockbackImpulseMagnitude = 500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RadialDamage")
	bool bIsRadialDamage = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RadialDamage")
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RadialDamage")
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RadialDamage")
	FVector RadialDamageOrigin = FVector::ZeroVector;
};
