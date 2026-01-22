// Copyright Dustin Carmelo


#include "Actor/AGASProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "AuraGAS/AuraGAS.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/AGASCombatInterface.h"
#include "Kismet/GameplayStatics.h"


AAGASProjectile::AAGASProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAGASProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);

	if (LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(
			LoopingSound,
			GetRootComponent(),
			NAME_None,
			FVector(ForceInit),
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
	
	if (HasAuthority() && ProjectileMovement->HomingTargetComponent.IsValid())
	{
		IAGASCombatInterface* HomingTargetCombatInterface = Cast<IAGASCombatInterface>(ProjectileMovement->HomingTargetComponent->GetOwner());
		if (HomingTargetCombatInterface)
		{
			HomingTargetCombatInterface->GetOnDeathDelegate().AddUniqueDynamic(this, &ThisClass::OnHomingTargetDeath);
		}
	}
}


void AAGASProjectile::Destroyed()
{
	// for clients in case the projectile is destroyed before OnSphereOverlap is called.
	if (!bHit && !HasAuthority())
	{
		HandleSpecialEffectsOnImpact();
	}
	Super::Destroyed();
}

void AAGASProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;

	if (!bHit)
	{
		HandleSpecialEffectsOnImpact();
	}

	if (HasAuthority())
	{
		// NOTE: We will only be using DamageEffectParams on the server since it was only set there, and it is not being
		// replicated
		// Check to make sure that the SourceASC is valid on the server
		check(DamageEffectParams.SourceAbilitySystemComponent)
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			FRotator ToTargetRotation = (OtherActor->GetActorLocation() - GetActorLocation()).Rotation();
			ToTargetRotation.Pitch = 45.f;
			const FVector ToTarget = ToTargetRotation.Vector();
			
			const FVector DeathImpulseVec = ToTarget * DamageEffectParams.DeathImpulseMagnitude;
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			DamageEffectParams.DeathImpulse = DeathImpulseVec;
			DamageEffectParams.KnockbackDirection = ToTarget;
			UAGASAbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
		}
		Destroy();
	}
	else
	{
		// Prevent the client from duplicating the projectile and running OnSphereOverlap again
		SetActorHiddenInGame(true);
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bHit = true;
	}
}

void AAGASProjectile::HandleSpecialEffectsOnImpact()
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	}

	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}
	
	bHit = true;
}

bool AAGASProjectile::IsValidOverlap(const AActor* OtherActor)
{
	const AActor* ThisOwner = GetOwner();
	if (!IsValid(ThisOwner))
	{
		bHit = true;
		Destroy();
		return false;
	}
	
	// Ignore Owner and prevent friendly fire
	if (OtherActor == ThisOwner || UAGASAbilitySystemLibrary::IsOnSameTeam(ThisOwner, OtherActor)) return false;
	
	return true;
}

void AAGASProjectile::OnHomingTargetDeath(AActor* DeadActor)
{
	// A fix for hovering projectiles after an actor dies
	// NOTE: this covers the case when a projectile is actively seeking the target, but it dies in the process
	FVector DeadActorLocationOnFloor;
	UAGASAbilitySystemLibrary::FindClosestLocationOnFloor(this, DeadActor->GetActorLocation(), DeadActorLocationOnFloor);
	HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
	HomingTargetSceneComponent->SetWorldLocation(DeadActorLocationOnFloor);
	ProjectileMovement->HomingTargetComponent = HomingTargetSceneComponent;
}
