// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASAbilityTypes.h"
#include "Data/AGASCharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AGASAbilitySystemLibrary.generated.h"

class UAGASAbilityInfo;
struct FWidgetControllerParams;
class UAGASSpellMenuWidgetController;
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
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAGASOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAGASAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAGASSpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAGASAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAGASAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|XPPoints")
	static int32 GetXPPointsRewarded(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const int32 CharacterLevel);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static UAGASCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|AbilityInfo", meta = (WorldContext = "WorldContextObject"))
	static UAGASAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockbackImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsSuccessfulDebuff);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffDamage);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffFrequency);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InDebuffDuration);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackImpulse);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffectToTarget(const FDamageEffectParams& InParams);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayMechanics", meta = (WorldContext = "WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayMechanics")
	static bool IsOnSameTeam(const AActor* FirstActor, const AActor* SecondActor);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

};
