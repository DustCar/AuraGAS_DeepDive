// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGA_CastBeam.h"

#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/AGASCombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAGASGA_CastBeam::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UAGASGA_CastBeam::StoreOwnerPlayerController()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = OwnerPlayerController->GetCharacter();
	}
}

void UAGASGA_CastBeam::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UAGASCombatInterface>())
	{
		if (USkeletalMeshComponent* WeaponMesh = IAGASCombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			
			FHitResult HitResult;
			
			const FVector SocketLocation = WeaponMesh->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter, 
				SocketLocation, 
				BeamTargetLocation, 
				10.f, 
				TraceTypeQuery1, 
				false, 
				ActorsToIgnore, 
				EDrawDebugTrace::None, 
				HitResult, 
				true
			);
			
			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
	
	if (IAGASCombatInterface* CombatInterface = Cast<IAGASCombatInterface>(MouseHitActor))
	{
		if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::PrimaryTargetDied)) return;
		
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::PrimaryTargetDied);
	}
}

void UAGASGA_CastBeam::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	
	// here we obtain all living targets in a large radius, before we cut it down to only a few close targets
	TArray<AActor*> OverlappingActors;
	UAGASAbilitySystemLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		AdditionalTargetRadius,
		MouseHitActor->GetActorLocation()
	);
	
	const int32 NumAdditionalTargets = FMath::Min(MaxNumShockTargets, GetAbilityLevel());
	UAGASAbilitySystemLibrary::GetClosestTargets(NumAdditionalTargets, OverlappingActors, OutAdditionalTargets, MouseHitActor->GetActorLocation());
	
	for (AActor* Target : OutAdditionalTargets)
	{
		if (IAGASCombatInterface* CombatInterface = Cast<IAGASCombatInterface>(Target))
		{
			if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::AdditionalTargetDied)) return;
			
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::AdditionalTargetDied);
		}
	}
}

void UAGASGA_CastBeam::RemoveOnDeathBindingFromPrimaryTarget()
{
	if (IAGASCombatInterface* CombatInterface = Cast<IAGASCombatInterface>(MouseHitActor))
	{
		CombatInterface->GetOnDeathDelegate().RemoveDynamic(this, &ThisClass::PrimaryTargetDied);
	}
}

void UAGASGA_CastBeam::RemoveOnDeathBindingFromAdditionalTarget(AActor* AdditionalTarget)
{
	if (IAGASCombatInterface* CombatInterface = Cast<IAGASCombatInterface>(AdditionalTarget))
	{
		CombatInterface->GetOnDeathDelegate().RemoveDynamic(this, &ThisClass::AdditionalTargetDied);
	}
}
