// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AGASEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURAGAS_API AAGASEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAGASEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	void RemoveInfiniteEffectsFromActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	bool bDestroyOnEffectApplication = false;
	
	//~ GameplayEffect Types and Policies Begin
	// Instant Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Duration Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	TArray<TSubclassOf<UGameplayEffect>> DurationGameplayEffectClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Infinite Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	TArray<TSubclassOf<UGameplayEffect>> InfiniteGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
	//~ GameplayEffect Types and Policies End

	/**
	 * Map that holds the TargetActor's UID as key and has a value of another map which holds the actors
	 * that apply the effects and what effects they apply.
	 *
	 * main map - key: TargetActor UID, value: Map of effect actors and their applied effects
	 * value map - key: EffectActor UID, value: Array of applied effects
	 *
	 * Tbh, this seems a little overkill for knowing what gameplay effects one actor is applying, but I can't
	 * think of anything else at the moment
	 */
	TMap<uint32, TMap<uint32, TArray<FActiveGameplayEffectHandle>>> ActiveInfiniteEffects;

};
