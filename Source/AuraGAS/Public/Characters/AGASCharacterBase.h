// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/AGASCharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interaction/AGASCombatInterface.h"
#include "AGASCharacterBase.generated.h"

class UAGASPassiveNiagaraComponent;
class UAGASDebuffNiagaraComponent;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAGASAttributeSet;
class UAGASAbilitySystemComponent;
class UMotionWarpingComponent;

// simple delegate to stop enemy movement when being shocked; alternative to previous GameplayTag events since
// there is no tag for being shocked, and we are just setting a bool bIsBeingShocked
DECLARE_DELEGATE_OneParam(FOnBeingShocked, bool)

UCLASS(Abstract)
class AURAGAS_API AAGASCharacterBase : public ACharacter, public IAbilitySystemInterface, public IAGASCombatInterface
{
	GENERATED_BODY()

public:
	AAGASCharacterBase();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAGASAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	/* Combat Interface functions */
	// Returns the hit react montage
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	// Pure function that handles any death functionality
	virtual void Die(const FVector& DeathImpulse) override;
	// Returns the location of a character's socket based on the tag passed in
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag) override;
	// Returns a bool on if the character is dead
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
	// Returns the number of current minions
	virtual int32 GetMinionCount_Implementation() override;
	// Add a positive or negative amount to minion count
	virtual void AddToMinionCount_Implementation(int32 Amount) override;
	// Get character class type
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	// Apply knockback to the character
	virtual void KnockbackCharacter_Implementation(const FVector& KnockbackForce) override;
	// Get the weapon mesh on the character, if one exists
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	// Set bIsBeingShocked bool
	virtual void SetIsBeingShocked_Implementation(bool bInIsBeingShocked) override;
	// Get bIsBeingShocked bool
	virtual bool IsBeingShocked_Implementation() const override;
	// Returns the delegate for OnASCRegistered
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	// Returns the delegate for OnDeath
	virtual FOnDeath& GetOnDeathDelegate() override;
	/* End Combat Interface functions */
	
	FOnASCRegistered OnAscRegistered;
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TArray<FTaggedMontage> AttackMontages;

	void SetWasSummoned(bool bInWasSummoned);
	
	UPROPERTY(ReplicatedUsing=OnRep_IsStunned, BlueprintReadOnly)
	bool bIsStunned = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_IsBurned, BlueprintReadOnly)
	bool bIsBurned = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;
	
	UFUNCTION()
	virtual void OnRep_IsStunned();
	
	UFUNCTION()
	virtual void OnRep_IsBurned();

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|CharacterClassDefaults")
	ECharacterClass CharacterClass;

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

	/* Minions */
	int32 MinionCount = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bWasSummoned = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Combat")
	float BaseWalkSpeed = 600.f;
	
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	FOnBeingShocked OnBeingShocked;

private:

	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAGASDebuffNiagaraComponent> BurnDebuffComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAGASDebuffNiagaraComponent> StunDebuffComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAGASPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAGASPassiveNiagaraComponent> LifeSiphonNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAGASPassiveNiagaraComponent> ManaSiphonNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
};
