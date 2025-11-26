// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AGASGameModeBase.generated.h"

class UAGASAbilityInfo;
class UAGASCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURAGAS_API AAGASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "CharacterClassDefaults")
	TObjectPtr<UAGASCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilityInfo")
	TObjectPtr<UAGASAbilityInfo> AbilityInfo;
};
