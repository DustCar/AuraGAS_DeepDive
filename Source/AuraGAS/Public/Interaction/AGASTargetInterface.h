// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AGASTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAGASTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAS_API IAGASTargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
