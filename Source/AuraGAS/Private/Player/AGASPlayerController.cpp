// Copyright Dustin Carmelo


#include "Player/AGASPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "Input/AGASInputConfig.h"
#include "Input/AGASInputComponent.h"
#include "Interaction/AGASTargetInterface.h"
#include "Player/AGASPlayerState.h"
#include "UI/HUD/AGASHUD.h"

AAGASPlayerController::AAGASPlayerController()
{
	bReplicates = true;
}

void AAGASPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
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
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (!Hit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Hit.GetActor();

	/**
	*	Line trace from cursor. Different Scenarios:
	*	A. LastActor is null && CurrentActor is null
	*		- Do nothing
	*	B. LastActor is null && CurrentActor is valid
	*		- Highlight CurrentActor
	*	C. LastActor is valid && CurrentActor is null
	*		- UnHighlight LastActor
	*	D. Both actors valid, but LastActor != CurrentActor
	*		- UnHighlight LastActor, Highlight CurrentActor
	*	E. Both actors valid, but LastActor == CurrentActor
	*		- Do nothing
	*/

	if (LastActor == nullptr)
	{
		// Case B
		if (CurrentActor)
		{
			CurrentActor->HighlightActor();
		}
	}
	else
	{
		if (CurrentActor)
		{
			// Case D
			if (LastActor != CurrentActor)
			{
				LastActor->UnHighlightActor();
				CurrentActor->HighlightActor();
			}
		}
		else
		{
			// Case C
			LastActor->UnHighlightActor();
		}
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
}

void AAGASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAGASInputComponent* AGASInputComponent = CastChecked<UAGASInputComponent>(InputComponent);
	AGASInputComponent->BindAction(AGASInputActions->InputMove, ETriggerEvent::Triggered, this, &ThisClass::Move);
	AGASInputComponent->BindAbilityActions(AGASInputActions, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAGASPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation(); // camera rotation, in our case since its top down 3rd person
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f); // rotator parallel to ground

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAGASPlayerController::AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AAGASPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void AAGASPlayerController::AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
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
