// Copyright Dustin Carmelo


#include "Player/AGASPlayerController.h"

#include "AGASGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "Actor/AGASMagicCircle.h"
#include "AuraGAS/AuraGAS.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/AGASCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AGASInputConfig.h"
#include "Input/AGASInputComponent.h"
#include "Interaction/AGASTargetInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/AGASPlayerState.h"
#include "UI/HUD/AGASHUD.h"
#include "UI/Widget/AGASDamageTextComponent.h"

AAGASPlayerController::AAGASPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	// Camera Occlusion
	CapsulePercentageForTrace = 1.0f;
	bDebugLineTraces = true;
	bIsOcclusionEnabled = true;
}

void AAGASPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	if (bAutoRunning)
	{
		AutoRun();
	}
	
	UpdateMagicCircleLocation();
}

void AAGASPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitializeHUD();
}

void AAGASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeHUD();
}

void AAGASPlayerController::ShowMagicCircle(float Radius, UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		FVector MagicCircleLocation = CursorHit.ImpactPoint;
		MagicCircle = GetWorld()->SpawnActor<AAGASMagicCircle>(MagicCircleClass, MagicCircleLocation, FRotator::ZeroRotator);
		MagicCircle->SetSphereRadius(Radius);
		if (DecalMaterial != nullptr)
		{
			MagicCircle->MagicCircleDecal->SetDecalMaterial(DecalMaterial);
		}
	}
}

void AAGASPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AAGASPlayerController::SetShowMouseCursorAndForceRefresh(bool bNewValue)
{
	SetShowMouseCursor(bNewValue);
	
	float XMouseLocation, YMouseLocation;
	GetMousePosition(XMouseLocation, YMouseLocation);
	SetMouseLocation(XMouseLocation, YMouseLocation);
}

void AAGASPlayerController::CursorTrace()
{
	// stop CursorTracing when the block tag is granted
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_CursorTrace))
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
			LastActor = nullptr;
		}
		if (CurrentActor)
		{
			CurrentActor->UnHighlightActor();
			CurrentActor = nullptr;
		}
		return;
	}
	
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludeActors : ECC_Target;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = CursorHit.GetActor();

	if (LastActor != CurrentActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (CurrentActor) CurrentActor->HighlightActor();
	}
}

void AAGASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AGASInputMapping);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AGASInputMapping, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (IsValid(GetPawn()))
	{
		ActiveSpringArm = Cast<USpringArmComponent>(GetPawn()->GetComponentByClass(USpringArmComponent::StaticClass()));
		ActiveCamera = Cast<UCameraComponent>(GetPawn()->GetComponentByClass(UCameraComponent::StaticClass()));
		ActiveCapsuleComponent = Cast<UCapsuleComponent>(GetPawn()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	}
}

void AAGASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAGASInputComponent* AGASInputComponent = CastChecked<UAGASInputComponent>(InputComponent);
	if (AGASInputComponent == nullptr) return;
	
	AGASInputComponent->BindAction(AGASInputActions->InputMove, ETriggerEvent::Triggered, this, &ThisClass::Move);
	AGASInputComponent->BindAction(AGASInputActions->InputShift, ETriggerEvent::Triggered, this, &ThisClass::ShiftPressed);
	AGASInputComponent->BindAbilityActions(AGASInputActions, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAGASPlayerController::Move(const FInputActionValue& Value)
{
	if (GetASC()->HasMatchingGameplayTag(TAG_Debuff_Stun)) return;
	
	if (bAutoRunning) bAutoRunning = false;
	
	const FVector2D InputAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		if (ActiveCamera == nullptr) return;
		const FRotator Rotation = ActiveCamera->GetComponentRotation(); // camera rotation, in our case since its top down 3rd person
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f); // rotator parallel to ground towards player

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Forward uses InputAxisVector.Y since that is W, S; Right uses .X since that is A, D
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

// Function that moves the player to cached destination through the whole spline given that bAutoRunning is true
void AAGASPlayerController::AutoRun()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		// literally just returns the closest point to the player
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		// then based on that point then we get the direction pointed to that point
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		// moves toward that point; we don't add any extra size since AutoRun is in tick so unit vector is fine
		ControlledPawn->AddMovementInput(Direction);

		// Length() turns the value Absolute in case you forget why this subtraction works no matter the values
		// plus we got the direction from earlier anyway. This is just to check if we stop running
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAGASPlayerController::AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputPressed))
	{
		bTargeting = false;
		bAutoRunning = false;
		return;
	}
	
	if (InputTag.MatchesTagExact(TAG_InputTag_LMB))
	{
		bTargeting = CurrentActor ? true : false;
		bAutoRunning = false;
	}
	
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AAGASPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputReleased))
	{
		return;
	}
	
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);

	if (InputTag.MatchesTagExact(TAG_InputTag_LMB))
	{
		if (!bTargeting && !bShiftKeyDown)
		{
			const APawn* ControlledPawn = GetPawn();
			if (FollowTime <= ShortPressThreshold && ControlledPawn)
			{
				FNavLocation CachedDestinationNavLocation;
				// padding to be more lenient with the path finding. Larger number less precise, smaller more precise
				const FVector QueryExtent = FVector(250.f, 250.f, 200.f);
				const FNavAgentProperties& NavAgentProperties = GetNavAgentPropertiesRef();
				// ProjectPointToNavigation finds the closest point on the nav mesh to the clicked location,
				// used for areas that do not have nav mesh (i.e. under an item)
				const bool bNavLocationFound = NavSystem->ProjectPointToNavigation(CachedDestination, CachedDestinationNavLocation, QueryExtent, &NavAgentProperties);

				if (!bNavLocationFound) return;
				
				UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestinationNavLocation);
				if (NavPath == nullptr || NavPath->PathPoints.Num() <= 0) return;

				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}

				CachedDestination = NavPath->PathPoints.Last();
				bAutoRunning = true;
				
				// make sure that we have a valid path before we call the click effect
				if (GetASC() && !GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputPressed))
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
				}
			}
		
			FollowTime = 0.f;
			bTargeting = false;
		}
	}
}

void AAGASPlayerController::AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputHeld))
	{
		return;
	}
	
	// checks if we are using mouse keys for abilities since they do other things; LMB for movement, RMB for moving camera
	const bool bMouseKeys = InputTag.MatchesTagExact(TAG_InputTag_LMB);

	// using mouse keys and not targeting or in "offensive" mode
	if (bMouseKeys && !bTargeting && !bShiftKeyDown)
	{
		// place this return for RMB since we don't want RMB to be movement
		if (InputTag.MatchesTagExact(TAG_InputTag_RMB)) return;
		
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Navigation, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
	// use ability that is tied to that input tag
	else
	{
		if (GetASC() == nullptr) return;
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}

void AAGASPlayerController::ShowDamageNumber_Implementation(const float DamageAmount, ACharacter* TargetCharacter, const bool bCriticalHit, const bool bBlockedHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UAGASDamageTextComponent* DamageText = NewObject<UAGASDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bCriticalHit, bBlockedHit);
	}
}

void AAGASPlayerController::SyncOccludedActors()
{
	if (!ShouldCheckCameraOcclusion()) return;

	// Camera is currently colliding, show all current occluded actors
	// and do not perform further occlusion
	if (ActiveSpringArm->bDoCollisionTest)
	{
		ForceShowOccludedActors();
		return;
	}

	FVector Start = ActiveCamera->GetComponentLocation();
	FVector End = GetPawn()->GetActorLocation() - (ActiveCamera->GetForwardVector() * DistanceFromActor);

	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	TArray<AActor*> ActorsToIgnore; // TODO: Add configuration to ignore actor types
	TArray<FHitResult> OutHits;

	auto ShouldDebug = bDebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		GetWorld(), Start, End, ActiveCapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
		ActiveCapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace * 0.5f, CollisionObjectTypes,
		true, ActorsToIgnore, ShouldDebug, OutHits, true);

	if (bGotHits)
	{
		// The list of actors hit by the line trace, that means that they are occluded from view
		TSet<const AActor*> ActorsJustOccluded;

		// Hide actors that are occluded by the camera
		for (FHitResult Hit : OutHits)
		{
			if (Hit.GetActor()->GetComponentByClass(UStaticMeshComponent::StaticClass()) == nullptr) continue;
			
			const TObjectPtr<AActor> HitActor = Cast<AActor>(Hit.GetActor());
			HideOccludedActor(HitActor);
			ActorsJustOccluded.Add(HitActor);
		}

		// Show actors that were occluded but no longer occluded after current pass
		for (auto& Elem : OccludedActors)
		{
			if (!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.bIsOccluded)
			{
				ShowOccludedActor(Elem.Value);

				if (bDebugLineTraces)
				{
					UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, but it's not occluded anymore with the new hits."),
						*Elem.Value.Actor->GetName());
				}
			}
		}
	}
	else
	{
		ForceShowOccludedActors();
	}
}

void AAGASPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		if (CursorHit.bBlockingHit)
		{
			MagicCircle->SetActorHiddenInGame(false);
			MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
		}
		else
		{
			MagicCircle->SetActorHiddenInGame(true);
		}
	}
}

bool AAGASPlayerController::HideOccludedActor(const AActor* Actor)
{
	FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);

	if (ExistingOccludedActor && ExistingOccludedActor->bIsOccluded)
	{
		if (bDebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was already occluded. Ignoring."),
			*Actor->GetName());
		return false;
	}

	if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
	{
		ExistingOccludedActor->bIsOccluded = true;
		OnHideOccludedActor(*ExistingOccludedActor);

		if (bDebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s exists, but was not occluded. Occluding it now."),
			*Actor->GetName());
	}
	else
	{
		UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		FCameraOccludedActor OccludedActor;
		OccludedActor.Actor = Actor;
		OccludedActor.StaticMesh = StaticMesh;
		OccludedActor.Materials = StaticMesh->GetMaterials();
		OccludedActor.bIsOccluded = true;
		OccludedActors.Add(Actor, OccludedActor);
		OnHideOccludedActor(OccludedActor);

		if (bDebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s does not exist, creating and occluding it now."),
			*Actor->GetName());
	}

	return true;
}

void AAGASPlayerController::ForceShowOccludedActors()
{
	for (auto& Elem : OccludedActors)
	{
		if (Elem.Value.bIsOccluded)
		{
			ShowOccludedActor(Elem.Value);

			if (bDebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, force to show again."),
			*Elem.Value.Actor->GetName());
		}
	}
}

void AAGASPlayerController::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
	if (!IsValid(OccludedActor.Actor))
	{
		OccludedActors.Remove(OccludedActor.Actor);
	}

	OccludedActor.bIsOccluded = false;
	OnShowOccludedActor(OccludedActor);
}

bool AAGASPlayerController::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
	{
		OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
		OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Target, ECR_Ignore);
		OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_ExcludeActors, ECR_Ignore);
	}

	return true;
}

bool AAGASPlayerController::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int matIdx = 0; matIdx < OccludedActor.Materials.Num(); ++matIdx)
	{
		OccludedActor.StaticMesh->SetMaterial(matIdx, OccludedActor.Materials[matIdx]);
		OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Target, ECR_Block);
		OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_ExcludeActors, ECR_Block);
	}

	return true;
}

void AAGASPlayerController::InitializeHUD()
{
	AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);

	if (AAGASHUD* AGASHUD = Cast<AAGASHUD>(GetHUD()))
	{
		AGASHUD->InitOverlay(this, AGASPlayerState, AGASPlayerState->GetAGASAbilitySystemComponent(), AGASPlayerState->GetAttributeSet());
	}
}

UAGASAbilitySystemComponent* AAGASPlayerController::GetASC()
{
	if (AGASAbilitySystemComponent == nullptr)
	{
		AGASAbilitySystemComponent = Cast<UAGASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AGASAbilitySystemComponent;
}
