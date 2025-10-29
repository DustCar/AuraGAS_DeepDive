// Copyright Dustin Carmelo


#include "AbilitySystem/Data/AGASLevelUpInfo.h"

#include "AuraGAS/AGASLogChannels.h"

int32 UAGASLevelUpInfo::FindLevelForXP(int32 XP, const bool bLogNotFoundVerbose) const
{
	// binary search to find level since level requirements are chronological
	int32 Start = 1;
	int32 End = LevelUpInformation.Num() - 1;
	while (Start <= End)
	{
		int32 Mid = (Start + End) / 2;
		if (XP < LevelUpInformation[Mid - 1].LevelUpRequirement)
		{
			End = Mid;
		}
		else if (XP < LevelUpInformation[Mid].LevelUpRequirement)
		{
			return Mid;
		}
		else
		{
			Start = Mid + 1;
		}
	}

	// by this point in the loop Start == End, most likely the last level
	// so if the XP passed in is less than the current highest level then they will be that level
	// else they would be considered passed that level since they would be greater than that requirement
	// (i.e. if level 5 is end is 5400, and we pass in 4000 then we return 5 since start == end)
	if (Start < End && XP < LevelUpInformation[Start].LevelUpRequirement)
	{
		return Start;
	}
	
	if (bLogNotFoundVerbose)
	{
		UE_LOG(LogAGAS, Error, TEXT("Can't find appropriate level for desired amount of XP: [%d]"), XP)
	}

	// if XP is greater than the highest level requirement then technically they will stop at that level despite
	// exceeding the requirement, since there is no level 6. Unless we want to increase by one but stop leveling after
	// the minus one is to compensate for the "else" condition in the binary search. This keeps the level at the max possible
	return Start - 1;
}
