// Copyright Dustin Carmelo


#include "Characters/AGASCharacterBase.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "AuraGAS/AuraGAS.h"


AAGASCharacterBase::AAGASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");
}

UAbilitySystemComponent* AAGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAGASCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAGASCharacterBase::Die()
{
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AAGASCharacterBase::MulticastHandleDeath_Implementation()
{
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}

void AAGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAGASCharacterBase::GetCombatSocketLocation_Implementation()
{
	check(WeaponMesh)
	return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
}

bool AAGASCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAGASCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAGASCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

void AAGASCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* WeaponDynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		WeaponMesh->SetMaterial(0, WeaponDynamicMatInst);
		StartWeaponDissolveTimeline(WeaponDynamicMatInst);
	}
}

void AAGASCharacterBase::InitializeAbilityActorInfo()
{
	
}

void AAGASCharacterBase::AddCharacterAbilities()
{
	
}

