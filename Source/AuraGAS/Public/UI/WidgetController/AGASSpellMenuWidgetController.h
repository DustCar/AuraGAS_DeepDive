// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASWidgetController.h"
#include "AGASSpellMenuWidgetController.generated.h"

class UAGASUserWidget;

// delegate that can be used to send a user widget
// for now we will be using it to bind all our spell globe buttons to it and 
// comparing the passed button to the button itself to see if it should select or deselect
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAGASWidgetSent, UAGASUserWidget*, UserWidget);

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASSpellMenuWidgetController : public UAGASWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellPoints")
	FOnPlayerStatChangedWidgetController OnPlayerSpellPointsChangedWidget;
	
	// the spell globe button will be binding to this and comparing itself to the passed in button
	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenu")
	FOnAGASWidgetSent OnAbilityButtonSelected;
	
	// will broadcast the ability button to the delegate
	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenu")
	void SelectedAbility(UAGASUserWidget* AbilityButton);

};
