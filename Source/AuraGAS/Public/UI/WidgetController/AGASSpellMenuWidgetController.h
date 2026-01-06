// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASWidgetController.h"
#include "AGASSpellMenuWidgetController.generated.h"

struct FGameplayTag;
class UAGASUserWidget;

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
	
	// will be used to call StopWaitingOnEquipSelection to stop animations when an ability is equipped
	UPROPERTY(BlueprintAssignable, Category = "GAS|SpellMenu")
	FOnEquippedAbility OnEquippedAbility;
	
	// function that the SpendPoint button in the Spell Menu will use
	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenu")
	void OnSpendPointButtonPressed(const FGameplayTag& AbilityTag);
	
	// function that the EquippedRowSpellGlobe buttons in the Spell Menu will use
	UFUNCTION(BlueprintCallable, Category = "GAS|SpellMenu")
	void EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& NewInputTag);

};
