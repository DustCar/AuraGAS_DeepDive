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

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
