// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASCharacterBase.h"
#include "Interaction/AGASTargetInterface.h"
#include "AGASEnemy.generated.h"

class AAGASAIController;
struct FGameplayTag;
class UWidgetComponent;
class UAGASProgressBarWidgetController;
class UBehaviorTree;

UCLASS()
class AURAGAS_API AAGASEnemy : public AAGASCharacterBase, public IAGASTargetInterface
{
	GENERATED_BODY()

public:
	AAGASEnemy();
	virtual void PossessedBy(AController* NewController) override;

	//~ Begin Target Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End Target Interface

	//~ Begin Combat Interface
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void SetCharacterLevel_Implementation(int32 NewLevel) override;
	virtual void Die() override;
	//~ End Combat Interface

	//~ Begin Target Interface
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	//~ End Target Interface

	UPROPERTY(BlueprintReadOnly, Category = "Setup|Combat")
	bool bHitReacting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Combat")
	float BaseWalkSpeed = 250.f;

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

private:
	
	void InitializeOverheadHealthBar();

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
};
