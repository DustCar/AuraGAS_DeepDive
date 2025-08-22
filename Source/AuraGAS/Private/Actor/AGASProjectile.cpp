// Copyright Dustin Carmelo


#include "Actor/AGASProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "AuraGAS/AuraGAS.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
	const AActor* ThisOwner = GetOwner();
	if (!IsValid(ThisOwner))
	{
		bHit = true;
		Destroy();
		return;
	}
	if (OtherActor == ThisOwner || UAGASAbilitySystemLibrary::IsOnSameTeam(ThisOwner, OtherActor)) return;

	if (!bHit)
	{
		HandleSpecialEffectsOnImpact();
	}

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
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
}
