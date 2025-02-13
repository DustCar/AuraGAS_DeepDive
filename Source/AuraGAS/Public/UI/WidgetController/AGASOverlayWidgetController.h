// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASWidgetController.h"
#include "AGASOverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURAGAS_API UAGASOverlayWidgetController : public UAGASWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaPointsChanged;

protected:
	void HealthPointsChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthPointsChanged(const FOnAttributeChangeData& Data) const;
	void ManaPointsChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaPointsChanged(const FOnAttributeChangeData& Data) const;
};
