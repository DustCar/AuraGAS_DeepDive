// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGASEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURAGAS_API AAGASEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAGASEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	UPROPERTY(EditAnywhere, Category = "AppliedEffects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
};
