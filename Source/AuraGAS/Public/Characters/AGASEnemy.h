// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AGASCharacterBase.h"
#include "AbilitySystem/Data/AGASCharacterClassInfo.h"
#include "Interaction/AGASTargetInterface.h"
#include "AGASEnemy.generated.h"

struct FGameplayTag;
class UWidgetComponent;
class UAGASProgressBarWidgetController;

UCLASS()
class AURAGAS_API AAGASEnemy : public AAGASCharacterBase, public IAGASTargetInterface
{
	GENERATED_BODY()

public:
	AAGASEnemy();

	//~ Begin Target Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End Target Interface

	//~ Begin Combat Interface
	virtual int32 GetPlayerLevel() override;
	//~ End Combat Interface

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterClassDefaults")
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterClassDefaults")
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAGASProgressBarWidgetController> OverheadHPBarWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAGASProgressBarWidgetController> OverheadHPBarWidgetController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:
	
	void InitializeOverheadHealthBar();

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
};
