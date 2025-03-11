// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASCharacterBase.h"
#include "AGASCharacter.generated.h"

UCLASS()
class AURAGAS_API AAGASCharacter : public AAGASCharacterBase
{
	GENERATED_BODY()

public:
	AAGASCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//~ Begin Combat Interface
	virtual int32 GetPlayerLevel() override;
	//~ End Combat Interface
	
protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;

private:

};
