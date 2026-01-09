// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AGASPlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAGASPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAS_API IAGASPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void AddToXPPointsOnPlayerState(int32 InXPPoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePointsOnPlayerState(int32 InAttributePoints);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPointsOnPlayerState(int32  InSpellPoints);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsOnPlayerState();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircleOnPlayerController(UMaterialInterface* DecalMaterial = nullptr);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircleOnPlayerController();
	
};
