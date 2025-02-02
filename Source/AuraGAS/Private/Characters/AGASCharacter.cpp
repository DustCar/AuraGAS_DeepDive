// Copyright Dustin Carmelo


#include "Characters/AGASCharacter.h"


AAGASCharacter::AAGASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAGASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


