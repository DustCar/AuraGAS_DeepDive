// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "Engine/DataAsset.h"
#include "AGASAttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// set only from C++
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute AttributeToGet;
};

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	FAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, const bool bLogNotFoundVerbose = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "AttributeName"))
	TArray<FAttributeInfo> AttributeInformation;
};
