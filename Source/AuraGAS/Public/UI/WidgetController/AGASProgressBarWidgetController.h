// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASWidgetController.h"
#include "AGASProgressBarWidgetController.generated.h"

class FOnAttributeChangedSignature;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURAGAS_API UAGASProgressBarWidgetController : public UAGASWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthPointsChanged;

private:

	bool bFirstBroadcast = true;
};
