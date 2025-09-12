// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/AGASCombatInterface.h"
#include "AGASCharacterBase.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAGASAttributeSet;
class UAGASAbilitySystemComponent;
class UMotionWarpingComponent;

UCLASS(Abstract)
class AURAGAS_API AAGASCharacterBase : public ACharacter, public IAbilitySystemInterface, public IAGASCombatInterface
{
	GENERATED_BODY()

public:
	AAGASCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAGASAttributeSet* GetAttributeSet() const { return AttributeSet; }

	
	/* Combat Interface functions */
	// Returns the hit react montage
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	
	// Pure function that handles any death functionality
	virtual void Die() override;
	
	// Returns the location of a character's socket based on the tag passed in
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag) override;
	
	virtual bool IsDead_Implementation() const override;
	
	// Returns the Avatar Actor
	virtual AActor* GetAvatar_Implementation() override;
	
	// Returns the whole array of attack montages a character has
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	
	// Returns one attack montage at random if there are more than 1; returns just the one if not
	virtual FTaggedMontage GetAttackMontageRandom_Implementation() override;
	
	// Returns the blood effect of the actor
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	
	// Returns impact sound of a characters attack with the specified montage tag
	virtual USoundBase* GetImpactSoundByMontageTag_Implementation(const FGameplayTag& InMontageTag) override;
	/* End Combat Interface functions */

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TArray<FTaggedMontage> AttackMontages;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo();
	virtual void AddCharacterAbilities();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	FName RightHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	FName TailSocketName;

	bool bDead = false;

	// Using just our version of ASC since we will not be implementing multiple types of ASC in this project
	UPROPERTY()
	TObjectPtr<UAGASAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAGASAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComp;

	/* Dissolve Effect */

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<USoundBase> DeathSound;

private:

	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
