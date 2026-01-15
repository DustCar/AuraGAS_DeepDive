// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGA_CastProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "Actor/AGASProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/AGASCombatInterface.h"


FString UAGASGA_CastProjectile::FormatDamageAbilityDescription(int32 Level, const FString& Description,
	UGameplayAbility* Ability)
{
	FString ParentDescription = Super::FormatDamageAbilityDescription(Level, Description, Ability);
	FStringFormatNamedArguments ProjNamesToValues;
	
	ProjNamesToValues.Add(TEXT("_ProjNum0"), FMath::Min(Level, MaxNumProjectiles));
	ProjNamesToValues.Add(TEXT("_ProjNum1"), FMath::Min(Level + 1, MaxNumProjectiles));
	
	FString NewProjDescription = FString::Format(*ParentDescription, ProjNamesToValues);
	
	return NewProjDescription;
}

void UAGASGA_CastProjectile::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (AvatarActor == nullptr || !AvatarActor->HasAuthority()) return;
	
	const FVector SocketLocation = IAGASCombatInterface::Execute_GetCombatSocketLocation(AvatarActor, SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	// SpawnActorDeferred is used to spawn projectile to delay the spawn call to allow us to set the
	// GameplayEffectContextHandle and to set the damage effect spec handle
	AAGASProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAGASProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
	Projectile->FinishSpawning(SpawnTransform);
}

void UAGASGA_CastProjectile::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	AActor* HomingTarget, const bool bOverridePitch, const float PitchAmount, const bool bHoming)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (AvatarActor == nullptr || !AvatarActor->HasAuthority()) return;
	
	const FVector SocketLocation = IAGASCombatInterface::Execute_GetCombatSocketLocation(AvatarActor, SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchAmount;
	
	const FVector Forward = Rotation.Vector();
	
	const int32 NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	
	TArray<FRotator> Rotations = UAGASAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);
	
	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		
		AAGASProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAGASProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		if (bHoming)
		{
			Projectile->ProjectileMovement->bIsHomingProjectile = true;
			if (HomingTarget && HomingTarget->Implements<UAGASCombatInterface>())
			{
				const bool bHomingTargetAlive = !IAGASCombatInterface::Execute_IsDead(HomingTarget);
				// if we have a target, and it's alive, set it as the target component
				if (bHomingTargetAlive)
				{
					Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
				}
				// else, we will create a scene component like the outer else case but this time the location will
				// be based on the previously alive targets location
				// NOTE: this case can happen if the projectile gets spawned right after a target actor dies and covers
				// the case where the OnDeath delegate doesn't get called
				else
				{
					FVector ProjectileTargetLocationOnFloor;
					UAGASAbilitySystemLibrary::FindClosestLocationOnFloor(this, ProjectileTargetLocation, ProjectileTargetLocationOnFloor);
					Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
					Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocationOnFloor);
					Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
				}
			}
			else
			{
				Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
				Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
				Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
			}
			Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		}
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
