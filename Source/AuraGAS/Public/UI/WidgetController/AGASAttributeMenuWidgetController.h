// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASWidgetController.h"
#include "AGASAttributeMenuWidgetController.generated.h"

struct FGameplayTag;
struct FAttributeInfo;
class UAGASAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAttributeInfo&, Info);

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASAttributeMenuWidgetController : public UAGASWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatChangedWidgetController OnPlayerAttributePointsChangedWidget;

	UFUNCTION(BlueprintCallable)
	void CallUpgradeAttribute(const FGameplayTag& AttributeTag);
	
protected:

	// Data asset that holds the array of AttributeInfos
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAGASAttributeInfo> AttributeInfo;


private:
	void BroadcastAttributeInfo(const FAttributeInfo& Info) const;

};
