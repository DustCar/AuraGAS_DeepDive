// Copyright Dustin Carmelo


#include "UI/HUD/AGASHUD.h"

#include "UI/Widget/AGASUserWidget.h"
#include "UI/WidgetController/AGASOverlayWidgetController.h"
#include "UI/WidgetController/AGASAttributeMenuWidgetController.h"

// Get current overlay widget controller, otherwise create one
UAGASOverlayWidgetController* AAGASHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	return GetWidgetController<UAGASOverlayWidgetController>(OverlayWidgetController, OverlayWidgetControllerClass, WCParams);
}

UAGASAttributeMenuWidgetController* AAGASHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	return GetWidgetController<UAGASAttributeMenuWidgetController>(AttributeMenuWidgetController, AttributeMenuWidgetControllerClass, WCParams);
}

// Creates the overlay widget and adds to viewport
void AAGASHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAGASAbilitySystemComponent* ASC, UAGASAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AGASHUD"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AGASHUD"))

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAGASUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UAGASOverlayWidgetController* OLWidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(OLWidgetController);
	//OLWidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}
