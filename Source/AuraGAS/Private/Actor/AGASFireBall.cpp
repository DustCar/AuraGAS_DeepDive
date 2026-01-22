// Copyright Dustin Carmelo


#include "Actor/AGASFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AGASGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AAGASFireBall::AAGASFireBall()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement->PrimaryComponentTick.bCanEverTick = false;
	ProjectileMovement->SetAutoActivate(false);
}

void AAGASFireBall::EmptyIgnoreList()
{
	IgnoreList.Empty();
}

void AAGASFireBall::BeginPlay()
{
	Super::BeginPlay();
	
	StartOutgoingTimeline();
}

void AAGASFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	
	// skips other actor if it has already been overlapped
	if (IgnoreList.Contains(OtherActor)) return;
	
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
			UAGASAbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
		}
		else
		{
			// Prevent the client from duplicating the projectile and running OnSphereOverlap again
			SetActorHiddenInGame(true);
			Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
		IgnoreList.Add(OtherActor);
	}
}

void AAGASFireBall::HandleSpecialEffectsOnImpact()
{
	// local gameplay cue execution. separate from the on destroyed version of the base projectile class
	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), TAG_GameplayCue_FireBlast, CueParams);
	}
	
	bHit = true;
}

