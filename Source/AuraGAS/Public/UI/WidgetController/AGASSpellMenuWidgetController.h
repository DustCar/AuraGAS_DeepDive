// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASWidgetController.h"
#include "AGASSpellMenuWidgetController.generated.h"

struct FGameplayTag;
class UAGASUserWidget;

// delegate that can be used to send a user widget
// for now we will be using it to bind all our spell globe buttons to it and 
// comparing the passed button to the button itself to see if it should select or deselect
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAGASWidgetSent, UAGASUserWidget*, UserWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquippedAbility);

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
	
	// will be used to call StopWaitingOnEquipSelection to stop animations when an ability is equipped
	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenu")
	FOnEquippedAbility OnEquippedAbility;
	
	// will broadcast the ability button to the delegate
	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenu")
	void SelectedAbility(UAGASUserWidget* AbilityButton);
	
	// function that the SpendPoint button in the Spell Menu will use
	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenu")
	void OnSpendPointButtonPressed(const FGameplayTag& AbilityTag);
	
	// function that the EquippedRowSpellGlobe buttons in the Spell Menu will use
	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenu")
	void EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& NewInputTag);

};
