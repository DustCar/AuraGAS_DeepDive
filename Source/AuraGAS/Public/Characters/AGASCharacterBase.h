// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AGASCharacterBase.generated.h"

UCLASS(Abstract)
class AURAGAS_API AAGASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAGASCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

};
