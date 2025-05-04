// Copyright Dustin Carmelo


#include "Input/AGASInputConfig.h"

const UInputAction* UAGASInputConfig::FindInputActionForTag(const FGameplayTag& InTag, const bool bLogNotFoundVerbose)
{
	for (const auto& [InputAction, InputTag]: AbilityInputActions)
	{
		if (InputAction && InputTag.MatchesTagExact(InTag))
		{
			return InputAction;
		}
	}

	if (bLogNotFoundVerbose)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find InputAction for Tag [%s] in AbilityInputActions array on InputConfig [%s]."), *InTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}


