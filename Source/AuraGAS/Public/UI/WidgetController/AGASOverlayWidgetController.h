// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASWidgetController.h"
#include "GameplayTagContainer.h"
#include "AGASOverlayWidgetController.generated.h"

class UAGASAbilityInfo;
struct FGameplayAttribute;
class UAGASUserWidget;

USTRUCT()
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAGASUserWidget> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image = nullptr;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSent, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityInfoSent, const FAbilityInfo&, Info);

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
	FOnAttributeChanged OnHealthPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnMaxHealthPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnManaPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnMaxManaPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FOnMessageWidgetRowSent MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FOnAbilityInfoSent AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChanged OnXPPointsPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnPlayerStatChangedWidgetController OnPlayerLevelChangedWidget;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WidgetData")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WidgetData")
	TObjectPtr<UAGASAbilityInfo> AbilityInfo;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	// callback function to bind to FAbilitiesGiven delegate from AGASAbilitySystemComponent
	void OnInitializedStartupAbilities();

	void OnXPPointsChanged(int32 NewXPPoints) const;
	
private:
	/**
	 *	Function that binds a gameplay attribute to the ASC gameplay attribute change delegate and
	 *	broadcasts the new value for the attribute to our custom OnAttributeChangedSignature delegate
	 */ 
	void BindAttributeChange(const FGameplayAttribute& Attribute, FOnAttributeChanged& AttributeDelegate) const;
};

template <typename T>
T* UAGASOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
