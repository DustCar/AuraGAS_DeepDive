// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGA_CastProjectile.h"

#include "AbilitySystemComponent.h"
#include "Actor/AGASProjectile.h"
#include "Interaction/AGASCombatInterface.h"


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
