// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AGASAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
