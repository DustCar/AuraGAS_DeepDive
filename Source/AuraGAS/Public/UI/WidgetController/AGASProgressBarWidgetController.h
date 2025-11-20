// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASWidgetController.h"
#include "AGASProgressBarWidgetController.generated.h"

class FOnAttributeChanged;
/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASProgressBarWidgetController : public UAGASWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnHealthPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnMaxHealthPointsChanged;

private:

	bool bFirstBroadcast = true;
};
