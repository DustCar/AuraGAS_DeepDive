// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "AGASPlayerController.generated.h"


class UNavigationSystemV1;
class USplineComponent;
class UAGASAbilitySystemComponent;
struct FInputActionInstance;
struct FGameplayTag;
class IAGASTargetInterface;
struct FInputActionValue;
class UAGASInputConfig;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AURAGAS_API AAGASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAGASPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void InitializeHUD();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> AGASInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAGASInputConfig> AGASInputActions;

	void Move(const FInputActionValue& Value);
	void ShiftPressed(const FInputActionValue& Value) { bShiftKeyDown = Value.Get<bool>(); }
	bool bShiftKeyDown = false;

	FHitResult CursorHit;
	void CursorTrace();
	TScriptInterface<IAGASTargetInterface> LastActor;
	TScriptInterface<IAGASTargetInterface> CurrentActor;

	void AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UAGASAbilitySystemComponent> AGASAbilitySystemComponent;

	UAGASAbilitySystemComponent* GetASC();

	UPROPERTY()
	UNavigationSystemV1* NavSystem = nullptr;

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
	
public:
	
	FORCEINLINE FHitResult GetCursorHit() { return CursorHit; }
	
};
