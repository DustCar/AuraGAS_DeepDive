// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AGASSaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAGASSaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAS_API IAGASSaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShouldLoadTransform();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadActor();
};
