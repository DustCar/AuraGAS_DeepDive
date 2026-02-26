
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
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 4;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 5;
		}
		if (bIsRadialDamage)
		{
			RepBits |= 1 << 6;
			
			if (RadialDamageInnerRadius > 0.f)
			{
				RepBits |= 1 << 7;
			}
			if (RadialDamageOuterRadius > 0.f)
			{
				RepBits |= 1 << 8;
			}
			if (!RadialDamageOrigin.IsZero())
			{
				RepBits |= 1 << 9;
			}
		}
		
	}

	Ar.SerializeBits(&RepBits, 10);

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
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = MakeShared<FGameplayTag>();
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 5))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << bIsRadialDamage;
		bIsRadialDamage = true;
		
		if (RepBits & (1 << 7))
		{
			Ar << RadialDamageInnerRadius;
		}
		if (RepBits & (1 << 8))
		{
			Ar << RadialDamageOuterRadius;
		}
		if (RepBits & (1 < 9))
		{
			RadialDamageOrigin.NetSerialize(Ar, Map, bOutSuccess);
		}
	}
	else
	{
		bIsRadialDamage = false;
	}
	
	return true;
}
