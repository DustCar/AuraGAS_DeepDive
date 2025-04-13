// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/AGASWidgetController.h"
#include "AGASHUD.generated.h"

class UAGASAttributeMenuWidgetController;
class UAGASWidgetController;
class UAGASAbilitySystemComponent;
class UAGASAttributeSet;
struct FWidgetControllerParams;
class UAGASOverlayWidgetController;
class UAGASUserWidget;
/**
 * 
 */
UCLASS()
class AURAGAS_API AAGASHUD : public AHUD
{
	GENERATED_BODY()

public:

	UAGASOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAGASAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAGASAbilitySystemComponent* ASC, UAGASAttributeSet* AS);

	template <class T>
	T* GetWidgetController(TObjectPtr<T>& WidgetController, const TSubclassOf<UAGASWidgetController>& WidgetControllerClass,	const FWidgetControllerParams& WCParams);

protected:
	
private:

	// Overlay Widget
	UPROPERTY()
	TObjectPtr<UAGASUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAGASUserWidget> OverlayWidgetClass;
	
	// Overlay Widget Controller
	UPROPERTY()
	TObjectPtr<UAGASOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAGASOverlayWidgetController> OverlayWidgetControllerClass;

	// Attribute Menu Widget Controller
	UPROPERTY()
	TObjectPtr<UAGASAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAGASAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};

template <typename T>
T* AAGASHUD::GetWidgetController(TObjectPtr<T>& WidgetController, const TSubclassOf<UAGASWidgetController>& WidgetControllerClass, const FWidgetControllerParams& WCParams)
{
	checkf(WidgetControllerClass, TEXT("WidgetControllerClass not specified in BP_AGASHUD"))

	if (WidgetController == nullptr)
	{
		WidgetController = NewObject<T>(this, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(WCParams);
		WidgetController->BindCallbacksToDependencies();
	}

	return WidgetController;
}
