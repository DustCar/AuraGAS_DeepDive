// Copyright Dustin Carmelo


#include "Player/AGASPlayerController.h"

#include "AGASGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AuraGAS/AuraGAS.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/AGASCharacter.h"
#include "Components/SplineComponent.h"
#include "Input/AGASInputConfig.h"
#include "Input/AGASInputComponent.h"
#include "Interaction/AGASTargetInterface.h"
#include "Player/AGASPlayerState.h"
#include "UI/HUD/AGASHUD.h"
#include "UI/Widget/AGASDamageTextComponent.h"

AAGASPlayerController::AAGASPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAGASPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	if (bAutoRunning)
	{
		AutoRun();
	}
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

void AAGASPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
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
}

void AAGASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAGASInputComponent* AGASInputComponent = CastChecked<UAGASInputComponent>(InputComponent);
	if (AGASInputComponent == nullptr) return;
	
	AGASInputComponent->BindAction(AGASInputActions->InputMove, ETriggerEvent::Triggered, this, &ThisClass::Move);
	AGASInputComponent->BindAction(AGASInputActions->InputShift, ETriggerEvent::Triggered, this, &ThisClass::ShiftPressed);
	AGASInputComponent->BindAction(AGASInputActions->InputRotateCamera, ETriggerEvent::Triggered, this, &ThisClass::RotateCamera);
	AGASInputComponent->BindAbilityActions(AGASInputActions, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAGASPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	

	if (AAGASCharacter* ControlledPawn = GetPawn<AAGASCharacter>())
	{
		const FRotator Rotation = ControlledPawn->GetCameraComponent()->GetComponentRotation(); // camera rotation, in our case since its top down 3rd person
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f); // rotator parallel to ground

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAGASPlayerController::RotateCamera(const FInputActionValue& Value)
{
	const float HorizontalMouseMovement = Value.Get<float>();

	if (AAGASCharacter* ControlledPawn = GetPawn<AAGASCharacter>())
	{
		ControlledPawn->GetSpringArmComponent()->AddRelativeRotation(FRotator(0.f, HorizontalMouseMovement * 5.f, 0.f));
	}
}

void AAGASPlayerController::AutoRun()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAGASPlayerController::AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(TAG_InputTag_LMB))
	{
		bTargeting = CurrentActor ? true : false;
		bAutoRunning = false;
	}
	
}

void AAGASPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
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
				const FVector QueryExtent = FVector(250.f, 250.f, 200.f);
				const FNavAgentProperties& NavAgentProperties = GetNavAgentPropertiesRef();
				// ProjectPointToNavigation finds the closest point on the nav mesh to the clicked location,
				// used for areas that do not have nav mesh (i.e. under an item)
				const bool bNavLocationFound = NavSystem->ProjectPointToNavigation(CachedDestination, CachedDestinationNavLocation, QueryExtent, &NavAgentProperties);

				if (bNavLocationFound)
				{
					UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestinationNavLocation);
					if (NavPath == nullptr || NavPath->PathPoints.Num() <= 0) return;

					Spline->ClearSplinePoints();
					for (const FVector& PointLoc : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					}

					CachedDestination = NavPath->PathPoints.Last();
					bAutoRunning = true;
				}
			}
		
			FollowTime = 0.f;
			bTargeting = false;
		}
	}
}

void AAGASPlayerController::AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(TAG_InputTag_LMB))
	{
		if (GetASC() == nullptr) return;
		GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC() == nullptr) return;
		GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
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

void AAGASPlayerController::InitializeHUD()
{
	AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);

	if (AAGASHUD* AGASHUD = Cast<AAGASHUD>(GetHUD()))
	{
		AGASHUD->InitOverlay(this, AGASPlayerState, Cast<UAGASAbilitySystemComponent>(AGASPlayerState->GetAbilitySystemComponent()), AGASPlayerState->GetAttributeSet());
	}
}

UAGASAbilitySystemComponent* AAGASPlayerController::GetASC()
{
	if (AGASAbilitySystemComponent == nullptr)
	{
		AGASAbilitySystemComponent = Cast<UAGASAbilitySystemComponent>(GetPlayerState<AAGASPlayerState>()->GetAbilitySystemComponent());
	}

	return AGASAbilitySystemComponent;
}
