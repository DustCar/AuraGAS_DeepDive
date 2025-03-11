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

	//~ Begin Combat Interface
	virtual int32 GetPlayerLevel() override;
	//~ End Combat Interface

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterClassDefaults")
	int32 Level = 1;
	
};
