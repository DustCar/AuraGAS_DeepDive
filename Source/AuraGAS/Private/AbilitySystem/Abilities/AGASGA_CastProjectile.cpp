// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGA_CastProjectile.h"

#include "AbilitySystemComponent.h"
#include "Actor/AGASProjectile.h"
#include "Interaction/AGASCombatInterface.h"


void UAGASGA_CastProjectile::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (AvatarActor == nullptr || !AvatarActor->HasAuthority()) return;

	if (AvatarActor->Implements<UAGASCombatInterface>())
	{
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

		// Set the projectile's GameplayEffectContextHandle
		const UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		// loop through all damage types that the ability has and set its magnitude
		for (auto Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			SpecHandle.Data->SetSetByCallerMagnitude(Pair.Key, ScaledDamage);
		}
		
		Projectile->DamageEffectSpecHandle = SpecHandle;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
