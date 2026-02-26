// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASGameplayAbility.h"
#include "AGASPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASPassiveAbility : public UAGASGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	float GetPercentAtLevel(int32 Level) const;
	
	void ReceivedDeactivate(const FGameplayTag& AbilityTag);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Passive")
	TSubclassOf<UGameplayEffect> PassiveEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Passive")
	FScalableFloat PercentEffective;
};
