// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AGASWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 *	Responsible for getting, calculating, and broadcasting data from the "model" (Ability System Component/Attribute Set)
 *	to the widgets on HUD
 */
UCLASS()
class AURAGAS_API UAGASWidgetController : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
