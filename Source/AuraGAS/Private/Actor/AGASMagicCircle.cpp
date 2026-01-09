// Copyright Dustin Carmelo


#include "Actor/AGASMagicCircle.h"

#include "Components/DecalComponent.h"


AAGASMagicCircle::AAGASMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleComponent");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AAGASMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGASMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

