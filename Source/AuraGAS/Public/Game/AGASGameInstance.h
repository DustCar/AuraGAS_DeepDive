// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AGASGameInstance.generated.h"

class UAGASAbilityInfo;
class UAGASCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "CharacterClassDefaults")
	TObjectPtr<UAGASCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilityInfo")
	TObjectPtr<UAGASAbilityInfo> AbilityInfo;
};
