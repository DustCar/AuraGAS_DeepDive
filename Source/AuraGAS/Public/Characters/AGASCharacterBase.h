// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/AGASCombatInterface.h"
#include "AGASCharacterBase.generated.h"

class UGameplayEffect;
class UAGASAttributeSet;
class UAGASAbilitySystemComponent;

UCLASS(Abstract)
class AURAGAS_API AAGASCharacterBase : public ACharacter, public IAbilitySystemInterface, public IAGASCombatInterface
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

	// Using just our version of ASC since we will not be implementing multiple types of ASC in this project
	UPROPERTY()
	TObjectPtr<UAGASAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAGASAttributeSet> AttributeSet;

	virtual void InitializeAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultMainStats;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryStats;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const;
	void InitializeDefaultStats() const;
};
