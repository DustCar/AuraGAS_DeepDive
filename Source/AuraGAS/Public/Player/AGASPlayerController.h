// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "AGASPlayerController.generated.h"


class AAGASMagicCircle;
class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UAGASDamageTextComponent;
class UNavigationSystemV1;
class USplineComponent;
class UAGASAbilitySystemComponent;
struct FInputActionInstance;
struct FGameplayTag;
class IAGASTargetInterface;
struct FInputActionValue;
class UAGASInputConfig;
class UInputMappingContext;
class UNiagaraSystem;

// Custom struct to store info for actors in the way of the camera
USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	const AActor* Actor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsOccluded = false;
};


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
	
	// damage text function
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(const float DamageAmount, ACharacter* TargetCharacter, const bool bCriticalHit, const bool bBlockedHit);
	
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(float Radius, UMaterialInterface* DecalMaterial = nullptr);
	
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	
	// small util function that helps refresh the cursor when hiding/showing in game
	void SetShowMouseCursorAndForceRefresh(bool bNewValue);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void InitializeHUD();

	// Camera Occlusion properties
	/*
	 * How much of the Pawn capsule Radius and Height should be used
	 * for the Line Trace before considering an Actor occluded?
	 * Values too low may make the camera clip through walls.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraOcclusion|Occlusion",
		meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float CapsulePercentageForTrace;

	// Pushes the end vector away from the character; Used in case larger capsules start hitting the ground
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraOcclusion|Occlusion",
		meta = (ClampMin = "0.0", ClampMax = "650.0"))
	float DistanceFromActor = 50.f;

	// Material to replace which should be more transparent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraOcclusion|Materials")
	TObjectPtr<UMaterialInstance> FadeMaterial;

	// Components of the player character; Does not create new ones, just holds the components of the player
	UPROPERTY(BlueprintReadWrite, Category = "CameraOcclusion|Components")
	TObjectPtr<USpringArmComponent> ActiveSpringArm;

	UPROPERTY(BlueprintReadWrite, Category = "CameraOcclusion|Components")
	TObjectPtr<UCameraComponent> ActiveCamera;

	UPROPERTY(BlueprintReadWrite, Category = "CameraOcclusion|Components")
	TObjectPtr<UCapsuleComponent> ActiveCapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraOcclusion")
	bool bIsOcclusionEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraOcclusion|Occlusion")
	bool bDebugLineTraces;

private:
	// Input
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> AGASInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAGASInputConfig> AGASInputActions;

	void Move(const FInputActionValue& Value);
	void ShiftPressed(const FInputActionValue& Value) { bShiftKeyDown = Value.Get<bool>(); }
	bool bShiftKeyDown = false;

	// Cursor Trace
	FHitResult CursorHit;
	void CursorTrace();
	TScriptInterface<IAGASTargetInterface> LastActor;
	TScriptInterface<IAGASTargetInterface> CurrentActor;

	// Input | Abilities
	void AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag);

	// Ability System Component
	UPROPERTY()
	TObjectPtr<UAGASAbilitySystemComponent> AGASAbilitySystemComponent;

	UAGASAbilitySystemComponent* GetASC();

	// Nav System for autorun
	UPROPERTY()
	UNavigationSystemV1* NavSystem = nullptr;

	// Autorun variables
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAGASDamageTextComponent> DamageTextComponentClass;

	// Camera Occlusion Properties
	UPROPERTY()
	TMap<const AActor*, FCameraOccludedActor> OccludedActors;

	// Occludes the actor given it exists, if not then creates it and occludes it
	bool HideOccludedActor(const AActor* Actor);
	
	// Replaces the actor's static mesh materials to the Fade material
	bool OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;

	// Shows actor if it was once occluded
	void ShowOccludedActor(FCameraOccludedActor& OccludedActor);
	
	// Reverts the actor's static mesh materials to its original
	bool OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	
	// forces the actor to be shown
	void ForceShowOccludedActors();

	FORCEINLINE bool ShouldCheckCameraOcclusion() const
	{
		return bIsOcclusionEnabled && FadeMaterial && ActiveCamera && ActiveCapsuleComponent;
	}

	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAGASMagicCircle> MagicCircleClass;
	
	UPROPERTY()
	TObjectPtr<AAGASMagicCircle> MagicCircle;
	
	void UpdateMagicCircleLocation();
	
public:
	
	FORCEINLINE FHitResult GetCursorHit() { return CursorHit; }
	
};
