// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AGASCharacterBase.generated.h"

class UAGASAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class AURAGAS_API AAGASCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAGASCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAGASAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAGASAttributeSet> AttributeSet;
};
