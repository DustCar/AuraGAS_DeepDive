// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAbilitySystemGlobals.h"

#include "AGASAbilityTypes.h"

FGameplayEffectContext* UAGASAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAGASGameplayEffectContext();
}
