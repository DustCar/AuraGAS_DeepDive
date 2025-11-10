// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayEffectTypes.h"
#include "AGASModifierDependencyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAGASModifierDependencyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAS_API IAGASModifierDependencyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FOnExternalGameplayModifierDependencyChange* GetOnModifierDependencyChanged() = 0;
	
};
