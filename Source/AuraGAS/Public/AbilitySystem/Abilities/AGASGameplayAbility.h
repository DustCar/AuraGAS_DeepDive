// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AGASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (Categories = "InputTag"))
	FGameplayTag StartupInputTag;
	
	static FString GetLockedDescription(int32 Level);
	
	int32 GetRoundedManaCost(const int32 InLevel = 1) const;
	float GetCooldown(const int32 InLevel = 1) const;
	
	
};
