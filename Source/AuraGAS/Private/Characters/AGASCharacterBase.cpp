// Copyright Dustin Carmelo


#include "Characters/AGASCharacterBase.h"


AAGASCharacterBase::AAGASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

