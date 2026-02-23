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
#include "Interaction/AGASHighlightInterface.h"
#include "Interaction/AGASTargetInterface.h"
#include "Kismet/GameplayStatics.h"
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

	if (bAutoRunEnabled && bAutoRunning)
	{
		AutoRun();
	}
	
	UpdateMagicCircleLocation();
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
		UnHighlightActor(LastActor);
		LastActor = nullptr;
		UnHighlightActor(CurrentActor);
		CurrentActor = nullptr;
		return;
	}
	
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludeActors : ECC_Target;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UAGASHighlightInterface>())
	{
		CurrentActor = CursorHit.GetActor();
	}
	else
	{
		CurrentActor = nullptr;
	}
	
	if (LastActor != CurrentActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(CurrentActor);
	}
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
		const FVector PawnLocation = ControlledPawn->GetActorLocation();
		// literally just returns the closest point to the player
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(PawnLocation, ESplineCoordinateSpace::World);
		// retrieve the point on the spline at Target index
		const FVector TargetSplinePointLocation = Spline->GetLocationAtSplinePoint(TargetSplinePointIdx, ESplineCoordinateSpace::World);
		FVector WorldDirection = TargetSplinePointLocation - PawnLocation;
		// zero the Z of the world direction to keep XY movement speed constant
		WorldDirection.Z = 0.f;
		// normalize it since its getting ran every tick so unit vectors are fine
		WorldDirection = WorldDirection.GetSafeNormal();
		
		ControlledPawn->AddMovementInput(WorldDirection);
		
		// Length() turns the value Absolute in case you forget why this subtraction works no matter the values
		// plus we got the direction from earlier anyway. This is just to check if we stop running
		const float DistanceToDestination = (LocationOnSpline - TargetSplinePointLocation).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			// even if within radius, rather than stopping abruptly, follow through to each point
			const bool bNextTargetPointExist = TargetSplinePointIdx < Spline->GetNumberOfSplinePoints() - 1;
			if (bNextTargetPointExist)
			{
				TargetSplinePointIdx++;
			}
			else
			{
				bAutoRunning = false;
			}
		}
	}
}

bool AAGASPlayerController::GetCursorPlaneIntersection(const FVector& InPlaneOrigin, const FVector& InPlaneNormal,
	FVector& OutPlanePoint) const
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D MousePosition;
		const bool bMousePositionFound = LocalPlayer->ViewportClient->GetMousePosition(MousePosition);
		if (bMousePositionFound)
		{
			return GetScreenPositionPlaneIntersection(MousePosition, InPlaneOrigin, InPlaneNormal, OutPlanePoint);
		}
	}
	return false;
}

bool AAGASPlayerController::GetScreenPositionPlaneIntersection(const FVector2D& ScreenPosition,
	const FVector& InPlaneOrigin, const FVector& InPlaneNormal, FVector& OutPlanePoint) const
{
	// if we hit a HUD menu then return early
	AHUD* HUD = GetHUD();
	if (HUD && HUD->GetHitBoxAtCoordinates(ScreenPosition, true))
	{
		return false;
	}
	
	// obtain the mouse position in world space
	FVector WorldOrigin;
	FVector WorldDirection;
	const bool bScreenPositionDeprojected = UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldOrigin, WorldDirection);
	if (bScreenPositionDeprojected)
	{
		// Point1 the deprojected position of our mouse cursor from our screen, Point2 is the same point as Point1 but quite
		// a distance away in the direction away from the camera (could think of it as the point under our cursor).
		// InPlaneOrigin is the location where we want the plane to sit, InPlaneNormal is the type of normal vector we want (usually UpVector)
		OutPlanePoint = FMath::LinePlaneIntersection(WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, InPlaneOrigin, InPlaneNormal);
		return true;
	}
	return false;
}

void AAGASPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UAGASHighlightInterface>())
	{
		IAGASHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AAGASPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UAGASHighlightInterface>())
	{
		IAGASHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}

void AAGASPlayerController::AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputPressed))
	{
		TargetingStatus = ETargetingStatus::NotTargeting;
		bAutoRunning = false;
		return;
	}
	
	if (InputTag.MatchesTagExact(TAG_InputTag_LMB))
	{
		ControlledPawnHalfHeight = Cast<IAGASCombatInterface>(GetPawn())->GetHalfHeight();
		if (IsValid(CurrentActor))
		{
			TargetingStatus = CurrentActor->Implements<UAGASTargetInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
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
		if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
		{
			const APawn* ControlledPawn = GetPawn();
			if (FollowTime <= ShortPressThreshold && ControlledPawn)
			{
				// Trace under the Navigation channel and use the Impact point as our desired location
				FHitResult NavChannelCursorHitResult;
				GetHitResultUnderCursor(ECC_Navigation, false, NavChannelCursorHitResult);
				
				if (IsValid(CurrentActor) && CurrentActor->Implements<UAGASHighlightInterface>())
				{
					IAGASHighlightInterface::Execute_SetMoveToLocation(CurrentActor, NavChannelCursorHitResult.ImpactPoint);
				}
				else if (GetASC() && !GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputPressed))
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, NavChannelCursorHitResult.ImpactPoint);
				}
				
				FNavLocation ImpactPointNavLocation;
				// padding to be more lenient with the path finding. Larger number less precise, smaller more precise
				const FVector QueryExtent = FVector(250.f, 250.f, 200.f);
				const FNavAgentProperties& NavAgentProperties = GetNavAgentPropertiesRef();
				// ProjectPointToNavigation finds the closest point on the nav mesh to the clicked location,
				// used for areas that do not have nav mesh (i.e. under an item)
				const bool bNavLocationFound = NavSystem->ProjectPointToNavigation(NavChannelCursorHitResult.ImpactPoint, ImpactPointNavLocation, QueryExtent, &NavAgentProperties);

				if (!bNavLocationFound) return;
				
				UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), ImpactPointNavLocation);
				if (NavPath == nullptr || NavPath->PathPoints.Num() <= 0) return;

				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}

				TargetSplinePointIdx = 1;
				bAutoRunning = true;
			}
		
			FollowTime = 0.f;
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
	}
}

void AAGASPlayerController::AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputHeld))
	{
		return;
	}

	// using mouse keys and not targeting or in "offensive" mode
	if (InputTag.MatchesTagExact(TAG_InputTag_LMB) && TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector PawnLocation = ControlledPawn->GetActorLocation();
			// obtain the location at the player character's feet, this will be used as the location for the invisible plane we want to project
			// for the press-to-move system
			FVector PawnFloorLocation = PawnLocation;
			PawnFloorLocation.Z -= ControlledPawnHalfHeight;
			FVector CursorHorizPlaneIntersection;
			const bool bIntersectionFound = GetCursorPlaneIntersection(PawnFloorLocation, FVector::UpVector, CursorHorizPlaneIntersection);
			if (bIntersectionFound)
			{
				FVector WorldDirection = (CursorHorizPlaneIntersection - PawnLocation).GetSafeNormal();
				WorldDirection.Z = 0.f;
				ControlledPawn->AddMovementInput(WorldDirection);
			}
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
		OccludedActor.OriginalCollisionProfileName = StaticMesh->GetCollisionProfileName();
		OccludedActor.bIsOccluded = true;
		OccludedActors.Add(Actor, OccludedActor);
		OnHideOccludedActor(OccludedActor);

		if (bDebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s does not exist, creating and occluding it now."),
			*Actor->GetName());
	}

	return true;
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

void AAGASPlayerController::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
	{
		OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
	}
	OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Target, ECR_Ignore);
	OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_ExcludeActors, ECR_Ignore);
	// after some testing, we will straight up make anything that is occluding "not hittable", including Navigation channel.
	// this will for sure create some bugs since some static meshes are tied together and can softlock movement for some meshes
	// with the original mouse movement system (I will be updating this movement system so it could get fixed).
	OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Navigation, ECR_Ignore);
}

void AAGASPlayerController::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int matIdx = 0; matIdx < OccludedActor.Materials.Num(); ++matIdx)
	{
		OccludedActor.StaticMesh->SetMaterial(matIdx, OccludedActor.Materials[matIdx]);
	}
	// OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Target, ECR_Block);
	// OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_ExcludeActors, ECR_Block);
	// OccludedActor.StaticMesh->SetCollisionResponseToChannel(ECC_Navigation, ECR_Block);
	OccludedActor.StaticMesh->SetCollisionProfileName(OccludedActor.OriginalCollisionProfileName);
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
