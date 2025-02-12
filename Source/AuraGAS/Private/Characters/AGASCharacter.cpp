// Copyright Dustin Carmelo


#include "Characters/AGASCharacter.h"

#include "AbilitySystemComponent.h"
#include "Player/AGASPlayerState.h"


AAGASCharacter::AAGASCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAGASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGASCharacter::InitializeAbilityActorInfo()
{
	AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);

	AbilitySystemComponent = AGASPlayerState->GetAbilitySystemComponent();
	AttributeSet = AGASPlayerState->GetAttributeSet();
	AbilitySystemComponent->InitAbilityActorInfo(AGASPlayerState, this);
}

void AAGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize Ability Actor Info for server
	InitializeAbilityActorInfo();
}

void AAGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize Ability Actor Info for client
	InitializeAbilityActorInfo();
}


