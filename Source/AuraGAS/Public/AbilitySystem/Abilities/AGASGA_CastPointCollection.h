// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASDamageGameplayAbility.h"
#include "AGASGA_CastPointCollection.generated.h"

class AAGASPointCollection;
/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASGA_CastPointCollection : public UAGASDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString FormatDamageAbilityDescription(int32 Level, const FString& Description, UGameplayAbility* Ability) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PointCollection")
	TSubclassOf<AAGASPointCollection> PointCollectionClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PointCollection")
	int32 MaxPointCount = 5;
};
