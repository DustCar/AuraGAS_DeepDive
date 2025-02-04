// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASCharacterBase.h"
#include "Interaction/AGASTargetInterface.h"
#include "AGASEnemy.generated.h"

UCLASS()
class AURAGAS_API AAGASEnemy : public AAGASCharacterBase, public IAGASTargetInterface
{
	GENERATED_BODY()

public:
	AAGASEnemy();

	//~ Begin Target Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End Target Interface

protected:
	virtual void BeginPlay() override;

};
