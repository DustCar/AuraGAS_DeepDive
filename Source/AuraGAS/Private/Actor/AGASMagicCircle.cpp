// Copyright Dustin Carmelo


#include "Actor/AGASMagicCircle.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Interaction/AGASTargetInterface.h"


AAGASMagicCircle::AAGASMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MagicCircleSphere = CreateDefaultSubobject<USphereComponent>("MagicCircleSphere");
	SetRootComponent(MagicCircleSphere);
	MagicCircleSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	MagicCircleSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleComponent");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AAGASMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
	MagicCircleSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMagicCircleBeginOverlap);
	MagicCircleSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnMagicCircleEndOverlap);
}

void AAGASMagicCircle::OnMagicCircleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAGASTargetInterface* TargetInterface = Cast<IAGASTargetInterface>(OtherActor))
	{
		TargetInterface->HighlightActor();
	}
}

void AAGASMagicCircle::OnMagicCircleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IAGASTargetInterface* TargetInterface = Cast<IAGASTargetInterface>(OtherActor))
	{
		TargetInterface->UnHighlightActor();
	}
}

void AAGASMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAGASMagicCircle::SetSphereRadius(float Radius)
{
	MagicCircleSphere->SetSphereRadius(Radius);
	MagicCircleDecal->DecalSize = FVector(Radius);
}

