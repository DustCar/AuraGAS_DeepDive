// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AGASWidgetController.generated.h"

class UAGASAbilityInfo;
class AAGASPlayerController;
class UAGASAttributeSet;
class UAGASAbilitySystemComponent;
class AAGASPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedWidgetController, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityInfoSent, const FAbilityInfo&, Info);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(AAGASPlayerController* PC, AAGASPlayerState* PS, UAGASAbilitySystemComponent* ASC, UAGASAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAGASPlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAGASPlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAGASAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAGASAttributeSet> AttributeSet = nullptr;
};
/**
 *	Responsible for getting, calculating, and broadcasting data from the "model" (Ability System Component/Attribute Set)
 *	to the widgets on HUD
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class AURAGAS_API UAGASWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	void BroadcastAbilityInfo();
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FOnAbilityInfoSent AbilityInfoDelegate;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AAGASPlayerController> AGASPlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AAGASPlayerState> AGASPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAGASAbilitySystemComponent> AGASAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAGASAttributeSet> AGASAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WidgetData")
	TObjectPtr<UAGASAbilityInfo> AbilityInfo;
};
