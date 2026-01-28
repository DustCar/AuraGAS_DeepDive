// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AGASLoadMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASLoadMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintInitializeWidget();
};
