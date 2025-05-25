// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/AGASCombatInterface.h"
#include "AGASCharacterBase.generated.h"

class UGameplayAbility;
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

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	virtual FVector GetCombactSocketLocation() override;

	// Using just our version of ASC since we will not be implementing multiple types of ASC in this project
	UPROPERTY()
	TObjectPtr<UAGASAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAGASAttributeSet> AttributeSet;

	virtual void InitializeAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const;
	void InitializeDefaultStats() const;

	void AddCharacterAbilities();

private:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
