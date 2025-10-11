// Copyright Dustin Carmelo


#include "AbilitySystem/Data/AGASAbilityInfo.h"

#include "AuraGAS/AGASLogChannels.h"

FAbilityInfo UAGASAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag,
                                                     const bool bLogNotFoundVerbose) const
{
	for (const FAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFoundVerbose)
	{
		UE_LOG(LogAGAS, Error, TEXT("Can't find info for Ability Tag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this))
	}

	return FAbilityInfo();
}
