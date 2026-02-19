// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASAbilityTypes.h"
#include "Data/AGASCharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AGASAbilitySystemLibrary.generated.h"

class UAGASLootTiers;
class UAGASLoadMenuSaveGame;
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
	
	/*
	 * Widget Controller 
	 */
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAGASOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAGASAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAGASSpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	/*
	 * Character Initialization
	 */
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAGASAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAGASAbilitySystemComponent* ASC, UAGASLoadMenuSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAGASAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	/*
	 * XP and Info getters
	 */
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|XPPoints")
	static int32 GetXPPointsRewarded(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const int32 CharacterLevel);

	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static UAGASCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|AbilityInfo", meta = (WorldContext = "WorldContextObject"))
	static UAGASAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|LootTiers", meta = (WorldContext = "WorldContextObject"))
	static UAGASLootTiers* GetLootTiers(const UObject* WorldContextObject);

	/*
	 * EffectContext getters
	 */
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
	static FVector GetKnockbackDirection(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

	/*
	 * Effect Context setters
	 */
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
	static void SetKnockbackDirection(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackDirection);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsRadialDamage);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InInnerRadius);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InOuterRadius);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin);
	
	/*
	 * ApplyDamage
	 */
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffectToTarget(const FDamageEffectParams& InParams);

	/*
	 * Gameplay Mechanics
	 */
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayMechanics", meta = (WorldContext = "WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);
	
	UFUNCTION(BlueprintCallable, Category = "AGASAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);

	/**
	 * Returns an approximate FVector location that is a blocking hit with the floor
	 * 
	 * @param WorldContextObject UObject to reference
	 * @param Origin Center location to trace from
	 * @param OutClosestLocation Out parameter holding the resultant FVector
	 * @param SearchHalfLength Number of UE units to search above/below the Origin
	 * @param ZOffset Custom offset to add to the final FVector (0 for default)
	 * @return Success boolean
	 */
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayMechanics", meta = (WorldContext = "WorldContextObject"))
	static bool FindClosestLocationOnFloor(const UObject* WorldContextObject, const FVector& Origin, FVector& OutClosestLocation, const float SearchHalfLength = 500.f, float ZOffset = 0.f);

	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayMechanics")
	static bool IsOnSameTeam(const AActor* FirstActor, const AActor* SecondActor);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
	
	UFUNCTION(BlueprintPure, Category = "AGASAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

};
