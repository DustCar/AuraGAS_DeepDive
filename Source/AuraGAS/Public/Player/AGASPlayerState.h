// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AGASPlayerState.generated.h"

class UAGASLevelUpInfo;
class UAGASAttributeSet;
class UAGASAbilitySystemComponent;

// no need to make it DYNAMIC since we only need it in C++ and the Widget Controllers would take care of sending values
// to widgets
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/)

/**
 * 
 */
UCLASS()
class AURAGAS_API AAGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAGASPlayerState();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAGASLevelUpInfo> LevelUpInfo;

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAGASAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FOnPlayerStatChanged OnLevelChangedSignature;
	FOnPlayerStatChanged OnXPPointsChangedSignature;

	void SetLevel(const int32 NewLevel);
	void SetXPPoints(const int32 NewXPPoints);

	void AddToLevel(const int32 InLevel);
	void AddToXPPoints(const int32 InXPPoints);

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

public:
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXPPoints() const { return XPPoints; }
};
