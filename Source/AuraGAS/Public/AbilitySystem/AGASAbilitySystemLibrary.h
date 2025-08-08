// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Data/AGASCharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AGASAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
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

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAGASAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static UAGASCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
};
