// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGameplayAbility.h"

FString UAGASGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Unlocked at Level %d.</>"), Level);
}
