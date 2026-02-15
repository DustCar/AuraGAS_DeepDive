// Copyright Dustin Carmelo


#include "Checkpoint/AGASCheckpoint.h"

#include "AuraGAS/AuraGAS.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Game/AGASGameModeBase.h"
#include "Interaction/AGASPlayerInterface.h"
#include "Kismet/GameplayStatics.h"


AAGASCheckpoint::AAGASCheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckpointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// position the bottom of the mesh to align with the bottom of the capsule
	const float CapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CheckpointMesh->SetRelativeLocation(FVector(350.f, 0.f, -CapsuleHalfHeight));
	
	Sphere->SetSphereRadius(350.f);
}

void AAGASCheckpoint::LoadActor_Implementation()
{
	if (bReached)
	{
		Glow();
	}
}

void AAGASCheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void AAGASCheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UAGASPlayerInterface>())
	{
		bReached = true;
		
		if (AAGASGameModeBase* AGASGameMode = Cast<AAGASGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AGASGameMode->SaveWorldState(GetWorld());
		}
		
		IAGASPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
        Glow();
	}
}

void AAGASCheckpoint::Glow()
{
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	CheckpointMesh->SetMaterial(0, DynamicMaterialInstance);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CheckpointReached(DynamicMaterialInstance);
}

