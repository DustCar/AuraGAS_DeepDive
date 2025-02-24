// Copyright Dustin Carmelo


#include "Characters/AGASCharacterBase.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"


AAGASCharacterBase::AAGASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AAGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGASCharacterBase::InitializeAbilityActorInfo()
{
	
}

