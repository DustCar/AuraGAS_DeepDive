// Copyright Dustin Carmelo


#include "Actor/AGASEnemySpawnVolume.h"

#include "Actor/AGASEnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interaction/AGASPlayerInterface.h"


AAGASEnemySpawnVolume::AAGASEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AAGASEnemySpawnVolume::LoadActor_Implementation()
{
	// For now, we will destroy the spawn volume after we reach it. 
	// I may refactor this so that enemies will respawn if you come back to a previous level like dark souls
	// if (bReached)
	// {
	// 	Destroy();
	// }
}

void AAGASEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxOverlap);
}

void AAGASEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UAGASPlayerInterface>()) return;
	
	bReached = true;
	for (AAGASEnemySpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		}
	}
	
	// Disable the volume to avoid spawning same volume multiple times in one level
	// Note: Traveling back to the level will reactivate the box, which is intended.
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


