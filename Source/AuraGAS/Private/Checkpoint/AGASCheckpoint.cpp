// Copyright Dustin Carmelo


#include "Checkpoint/AGASCheckpoint.h"

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
	
	// We want to be able to occlude the checkpoint mesh (our occlude system only affect WorldStatic object types), and not let it affect navigation
	if (bShouldOcclude)
	{
		CheckpointMesh->SetCollisionObjectType(ECC_WorldStatic);
		CheckpointMesh->SetCollisionResponseToChannel(ECC_Navigation, ECR_Ignore);
	}
	
	CheckpointMesh->SetCustomDepthStencilValue(CustomDepthStencilOverride);
	CheckpointMesh->MarkRenderStateDirty();
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckpointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetCanEverAffectNavigation(false);
	Sphere->SetCollisionObjectType(ECC_WorldStatic);
	
	MoveToComponent = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment(GetRootComponent());
	
	// position the bottom of the mesh to align with the bottom of the capsule
	const float CapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CheckpointMesh->SetRelativeLocation(FVector(350.f, 0.f, -CapsuleHalfHeight));
	
	Sphere->SetSphereRadius(300.f);
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
	
	if (bBindOverlapCallback)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	}
}

void AAGASCheckpoint::HighlightActor_Implementation()
{
	if (!bReached)
	{
		CheckpointMesh->SetRenderCustomDepth(true);
	}
}

void AAGASCheckpoint::UnHighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(false);
}

FVector AAGASCheckpoint::GetMoveToLocation()
{
	return MoveToComponent->GetComponentLocation();
}

void AAGASCheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UAGASPlayerInterface>())
	{
		bReached = true;
		
		if (AAGASGameModeBase* AGASGameMode = Cast<AAGASGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			const UWorld* World = GetWorld();
			FString MapName = World->GetMapName();
			MapName.RemoveFromStart(World->StreamingLevelsPrefix);
			
			AGASGameMode->SaveWorldState(GetWorld(), MapName);
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

