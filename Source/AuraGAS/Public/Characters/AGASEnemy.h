// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASCharacterBase.h"
#include "Interaction/AGASHighlightInterface.h"
#include "Interaction/AGASTargetInterface.h"
#include "AGASEnemy.generated.h"

class AAGASAIController;
struct FGameplayTag;
class UWidgetComponent;
class UAGASProgressBarWidgetController;
class UBehaviorTree;

UCLASS()
class AURAGAS_API AAGASEnemy : public AAGASCharacterBase, public IAGASTargetInterface, public IAGASHighlightInterface
{
	GENERATED_BODY()

public:
	AAGASEnemy();
	virtual void PossessedBy(AController* NewController) override;

	//~ Begin Highlight Interface
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	//~ End Highlight Interface

	//~ Begin Combat Interface
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void SetCharacterLevel_Implementation(int32 NewLevel) override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void KnockbackCharacter_Implementation(const FVector& KnockbackForce) override;
	//~ End Combat Interface

	//~ Begin Target Interface
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	//~ End Target Interface

	UPROPERTY(BlueprintReadOnly, Category = "Setup|Combat")
	bool bHitReacting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Combat")
	float CorpseLifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Setup|Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "Setup|CharacterClassDefaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Category = "Setup|UI")
	TSubclassOf<UAGASProgressBarWidgetController> OverheadHPBarWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAGASProgressBarWidgetController> OverheadHPBarWidgetController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|AI")
	TObjectPtr<UBehaviorTree> EnemyBehaviorTree;

	UPROPERTY()
	TObjectPtr<AAGASAIController> AGASAIController;
	
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

private:
	
	void InitializeOverheadHealthBar();

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
};
