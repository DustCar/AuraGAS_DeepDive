// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interaction/AGASModifierDependencyInterface.h"
#include "AGASPlayerState.generated.h"

class UAGASLevelUpInfo;
class UAGASAttributeSet;
class UAGASAbilitySystemComponent;

// no need to make it DYNAMIC since we only need it in C++ and the Widget Controllers would take care of sending values
// to widgets
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StatValue*/, bool /*bLevelUp*/);

/**
 * 
 */
UCLASS()
class AURAGAS_API AAGASPlayerState : public APlayerState, public IAbilitySystemInterface, public IAGASModifierDependencyInterface
{
	GENERATED_BODY()

public:
	AAGASPlayerState();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAGASLevelUpInfo> LevelUpInfo;

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAGASAbilitySystemComponent* GetAGASAbilitySystemComponent() const;
	UAGASAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FOnLevelChanged OnLevelChangedSignature;
	FOnPlayerStatChanged OnXPPointsChangedSignature;
	FOnPlayerStatChanged OnAttributePointsChangedSignature;
	FOnPlayerStatChanged OnSpellPointsChangedSignature;

	void SetLevel(const int32 NewLevel);
	void SetXPPoints(const int32 NewXPPoints);
	void SetAttributePoints(const int32 NewAttributePoints);
	void SetSpellPoints(const int32 NewSpellPoints);

	void AddToLevel(const int32 InLevel);
	void AddToXPPoints(const int32 InXPPoints);
	void AddToAttributePoints(const int32 InAttributePoints);
	void AddToSpellPoints(const int32 InSpellPoints);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAGASAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAGASAttributeSet> AttributeSet;
	
private:
    
    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
    int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XPPoints)
	int32 XPPoints = 0;

	UFUNCTION()
	void OnRep_XPPoints(int32 OldXPPoints);

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

	FOnExternalGameplayModifierDependencyChange OnModifierDependencyChanged;

public:
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXPPoints() const { return XPPoints; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	FORCEINLINE virtual FOnExternalGameplayModifierDependencyChange* GetOnModifierDependencyChanged() override { return &OnModifierDependencyChanged; }
};
