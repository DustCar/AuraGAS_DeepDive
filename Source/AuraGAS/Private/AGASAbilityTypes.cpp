
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
	}

	Ar.SerializeBits(&RepBits, 2);

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
	
	return true;
}
