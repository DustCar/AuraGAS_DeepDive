// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AGASLevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointsReward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointsReward = 1;
};

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASLevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLevelUpInfo> LevelUpInformation;

	int32 FindLevelForXP(int32 XP, const bool bLogNotFoundVerbose = false) const;
};
