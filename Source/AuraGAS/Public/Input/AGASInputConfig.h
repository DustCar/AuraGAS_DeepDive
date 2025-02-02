// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AGASInputConfig.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputMove;
	
};
