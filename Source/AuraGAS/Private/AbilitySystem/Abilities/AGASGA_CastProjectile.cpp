// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGA_CastProjectile.h"

#include "Actor/AGASProjectile.h"
#include "Interaction/AGASCombatInterface.h"


void UAGASGA_CastProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthority(&ActivationInfo))
	{
		return;
	}

	IAGASCombatInterface* CombatInterface = Cast<IAGASCombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombactSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		// TODO: Set the Projectile Rotation
		
		AAGASProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAGASProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		// TODO: Give the Projectile a Gameplay Effect Spec for causing Damage

		Projectile->FinishSpawning(SpawnTransform);
	}
	
	
}
