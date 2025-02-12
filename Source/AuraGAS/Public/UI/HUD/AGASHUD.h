// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AGASHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
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

	UPROPERTY()
	TObjectPtr<UAGASUserWidget> OverlayWidget;

	UAGASOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
protected:
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAGASUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UAGASOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAGASOverlayWidgetController> OverlayWidgetControllerClass;
};
