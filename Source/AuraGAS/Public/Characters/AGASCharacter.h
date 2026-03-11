// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASCharacterBase.h"
#include "Interaction/AGASPlayerInterface.h"
#include "AGASCharacter.generated.h"

class UAGASRespawnMessageWidgetController;
class UAGASUserWidget;
class UAGASWidgetController;
class UWidgetComponent;
class UNiagaraComponent;
class USpringArmComponent;
class UCameraComponent;
struct FGameplayEffectContextHandle;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeathTimer, float /*NewValue*/);

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
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void KnockbackCharacter_Implementation(const FVector& KnockbackForce) override;
	//~ End Combat Interface

	//~ Begin Player Interface
	virtual void AddToXPPointsOnPlayerState_Implementation(int32 InXPPoints) override;
	virtual void AddToAttributePointsOnPlayerState_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPointsOnPlayerState_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePointsOnPlayerState_Implementation() override;
	virtual void ShowMagicCircleOnPlayerController_Implementation(float Radius, UMaterialInterface* DecalMaterial) override;
	virtual void HideMagicCircleOnPlayerController_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	//~ End Player Interface
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DeathTime = 5.f;
	
	FTimerHandle DeathTimerHandle;
	
	FOnDeathTimer OnDeathTimerSent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
	virtual void MulticastHandleDeath_Implementation(const FVector& DeathImpulse) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void AddCharacterAbilities() override;
	virtual void InitializeAbilityActorInfo() override;

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
	
	UPROPERTY(EditAnywhere, Category = "Setup|UI")
	TSubclassOf<UAGASRespawnMessageWidgetController> RespawnMessageWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAGASRespawnMessageWidgetController> RespawnMessageWidgetController;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAGASUserWidget> RespawnMessageWidgetClass;

	void InitializeDefaultAttributes() const;
	void LoadProgress();

private:
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	// New function for applying effect to self
	void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const FGameplayEffectContextHandle& EffectContextHandle, float Level = 1.0f) const;
	
	void InitializeRespawnWidgetController();
};
