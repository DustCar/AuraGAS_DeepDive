
#include "AGASAbilityTypes.h"

bool FAGASGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	Super::NetSerialize(Ar, Map, bOutSuccess);

	uint16 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 0;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 1;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 2;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 3;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 4;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 5;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 6;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 7;
		}
		if (!KnockbackImpulse.IsZero())
		{
			RepBits |= 1 << 8;
		}
	}

	Ar.SerializeBits(&RepBits, 9);

	if (RepBits & (1 << 0))
	{
		Ar << bIsCriticalHit;
		bIsCriticalHit = true;
	}
	else
	{
		bIsCriticalHit = false;
	}
	if (RepBits & (1 << 1))
	{
		Ar << bIsBlockedHit;
		bIsBlockedHit = true;
	}
	else
	{
		bIsBlockedHit = false;
	}
	if (RepBits & (1 << 2))
	{
		Ar << bIsSuccessfulDebuff;
		bIsSuccessfulDebuff = true;
	}
	else
	{
		bIsSuccessfulDebuff = false;
	}
	if (RepBits & (1 << 3))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 4))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 5))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 6))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = MakeShared<FGameplayTag>();
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 7))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 8))
	{
		KnockbackImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	
	return true;
}
