// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AGASAbilityInfo.generated.h"


class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly)
	int32 AbilityLevel = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AbilityName = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AbilityDescription =  FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText NextLevelDescription =  FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
};

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	FAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, const bool bLogNotFoundVerbose = false) const;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "AbilityTag"))
	TArray<FAbilityInfo> AbilityInformation;
};
