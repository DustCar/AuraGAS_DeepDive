// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
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
	
	int32 GetRoundedDamageAtLevel(int32 Level, const FGameplayTag& DamageTag);
	static FString FormatDamageAbilityDescription(int32 Level, const FString& Description, UGameplayAbility* Ability);
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage");
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
