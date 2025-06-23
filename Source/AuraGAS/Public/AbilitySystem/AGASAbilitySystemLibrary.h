// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Data/AGASCharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AGASAbilitySystemLibrary.generated.h"

class UAGASAbilitySystemComponent;
class UAGASAttributeMenuWidgetController;
class UAGASOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAGASOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAGASAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAGASAbilitySystemComponent* ASC);
};
