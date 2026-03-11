// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASWidgetController.h"
#include "AGASRespawnMessageWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathTimerUpdated, float, NewValue);

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASRespawnMessageWidgetController : public UAGASWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnDeathTimerUpdated OnDeathTimerUpdated;
};
