// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AGASUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
