// Copyright Dustin Carmelo


#include "AbilitySystem/Data/AGASAttributeInfo.h"

#include "AuraGAS/AGASLogChannels.h"

FAttributeInfo UAGASAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,
                                                           const bool bLogNotFoundVerbose) const
{
	for (const FAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFoundVerbose)
	{
		UE_LOG(LogAGAS, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAttributeInfo();
}
