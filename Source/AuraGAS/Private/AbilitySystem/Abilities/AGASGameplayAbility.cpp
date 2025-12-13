// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGameplayAbility.h"

#include "AbilitySystem/AGASAttributeSet.h"

FString UAGASGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Unlocked at Level %d.</>"), Level);
}

int32 UAGASGameplayAbility::GetRoundedManaCost(const int32 InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (const auto& Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAGASAttributeSet::GetManaPointsAttribute())
			{
				// GetStaticMagnitude only works with a scalable float or a hard coded value from a curve
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	
	// have to use abs since cost GE magnitude is negative at base
	return FMath::Abs(FMath::RoundHalfToEven(ManaCost));
}

float UAGASGameplayAbility::GetCooldown(const int32 InLevel) const
{
	float CooldownTime = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CooldownTime);
	}
	
	return CooldownTime;
}
