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

protected:
	virtual void BeginPlay() override;
	void InitializeAbilityActorInfo();

private:

};
