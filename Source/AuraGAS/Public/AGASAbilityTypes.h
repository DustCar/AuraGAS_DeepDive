#pragma once

#include "GameplayEffectTypes.h"
#include "AGASAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAGASGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	
	/* Returns the actual struct used for serialization */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FAGASGameplayEffectContext::StaticStruct();
	}

	/* Custom serialization */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
protected:

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsBlockedHit = false;
	
};

