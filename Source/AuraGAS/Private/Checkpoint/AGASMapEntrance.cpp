// Copyright Dustin Carmelo


#include "Checkpoint/AGASMapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AGASGameModeBase.h"
#include "Interaction/AGASPlayerInterface.h"
#include "Kismet/GameplayStatics.h"


AAGASMapEntrance::AAGASMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	Sphere->SetupAttachment(MoveToComponent);
}

void AAGASMapEntrance::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGASMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UAGASPlayerInterface>())
	{
		bReached = true;
		
		if (AAGASGameModeBase* AGASGameMode = Cast<AAGASGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AGASGameMode->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		
		IAGASPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
		
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}

void AAGASMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void AAGASMapEntrance::LoadActor_Implementation()
{
	// Do nothing for now when loading map entrance
}

