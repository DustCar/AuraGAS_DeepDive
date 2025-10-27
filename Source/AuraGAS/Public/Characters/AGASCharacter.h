// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASCharacterBase.h"
#include "Interaction/AGASPlayerInterface.h"
#include "AGASCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FGameplayEffectContextHandle;

UCLASS()
class AURAGAS_API AAGASCharacter : public AAGASCharacterBase, public IAGASPlayerInterface
{
	GENERATED_BODY()

public:
	AAGASCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//~ Begin Combat Interface
	virtual int32 GetCharacterLevel_Implementation() override;
	//~ End Combat Interface

	//~ Begin Player Interface
	virtual void AddToXPPointsOnPlayerState_Implementation(int32 InXPPoints) override;
	//~ End Player Interface
	
protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	virtual void AddCharacterAbilities() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Setup|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Setup|Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	void InitializeDefaultStats() const;


private:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	// Old function for applying effect to self
	// void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const;
	
	// New function for applying effect to self
	void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const FGameplayEffectContextHandle& EffectContextHandle, float Level = 1.0f) const;
};
