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
	
	void ReceivedDeactivate(const FGameplayTag& AbilityTag);
};
