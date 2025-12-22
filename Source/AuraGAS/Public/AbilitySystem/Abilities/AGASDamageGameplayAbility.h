// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASAbilityTypes.h"
#include "AGASGameplayAbility.h"
#include "AGASDamageGameplayAbility.generated.h"

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
	
	int32 GetRoundedDamageAtLevel(int32 Level);
	static FString FormatDamageAbilityDescription(int32 Level, const FString& Description, UGameplayAbility* Ability);
	
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (Categories = "Damage"));
	FGameplayTag DamageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage");
	FScalableFloat DamageFloat;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff");
	float DebuffChance = 20.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff");
	float DebuffDamage = 5.f;
	
	// you can think of this as the tick for DOTs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff");
	float DebuffFrequency = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debuff");
	float DebuffDuration = 5.f;
};
