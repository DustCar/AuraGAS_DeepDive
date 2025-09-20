// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGA_CastSummon.h"

#include "Characters/AGASCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

TArray<FVector> UAGASGA_CastSummon::GetSpawnLocations()
{
	const FVector OwnerForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();

	if (NumMinions <= 0)
	{
		return TArray<FVector>();
	}
	const float DeltaSpread = SpawnSpreadAngle / NumMinions;

	// -SpreadAngle * 0.5f to create a cone in the center. Without 0.5f the cone would be off-center
	// since the ability would choose to start from the actor's 90 deg to the opposite 90 deg
	const FVector LeftOfSpread = OwnerForward.RotateAngleAxis(-SpawnSpreadAngle * 0.5f, FVector::UpVector);
	TArray<FVector> SpawnLocations{};
	for (int32 i = 0; i < NumMinions; i++)
	{
		// DeltaSpread * i: Angle at current i; ex. DeltaSpread = 90 / 5 = 18; i = 2; Angle at 2 = 18 * 2 = 36
		// + DeltaSpread * 0.5f: Spawn points are spread an additional half spread from the edges and from each other
		// this combination allows for even spread but doesn't spawn from the outermost edges
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i + DeltaSpread * 0.5f, FVector::UpVector);
		// Chooses a random location on the "lines" within spawn distance min max
		FVector ChosenSpawnLocation = OwnerLocation + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.Location;
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
	}

	return SpawnLocations;
}

TSubclassOf<AAGASCharacterBase> UAGASGA_CastSummon::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}

void UAGASGA_CastSummon::SpawnMinion(const FVector& SpawnLocation)
{
	// AvatarActor is the summoner, cast to AGASCharacterBase to obtain Level
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const TSubclassOf<AAGASCharacterBase> MinionClass = GetRandomMinionClass();

	const bool bNotValid = AvatarActor == nullptr ||
		!AvatarActor->Implements<UAGASCombatInterface>() ||
		!AvatarActor->HasAuthority() ||
		MinionClass == nullptr;
	if (bNotValid) return;
	
	const AAGASCharacterBase* MinionDefaultObject = MinionClass.GetDefaultObject();

	FTransform MinionTransform;
	MinionTransform.SetLocation(SpawnLocation + FVector(0.f, 0.f, MinionDefaultObject->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	MinionTransform.SetRotation(UKismetMathLibrary::FindLookAtRotation(AvatarActor->GetActorLocation(), SpawnLocation).Quaternion());
	
	// the Location is adjusted by half the height of the character's capsule height so that they spawn snapped to the floor.
	// Rotation is obtained by targeting the summoner's location then "moving" towards the minion's location.
	// Instigator for the spawn actor is the summoner. I think this means that the summoner can take credit for damage the
	// spawned actor does
	AAGASCharacterBase* SpawnedCharacter = GetWorld()->SpawnActorDeferred<AAGASCharacterBase>(
		MinionClass,
		MinionTransform,
		AvatarActor,
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding
	);

	SpawnedCharacter->SetWasSummoned(true);
	IAGASCombatInterface::Execute_SetCharacterLevel(SpawnedCharacter, IAGASCombatInterface::Execute_GetCharacterLevel(AvatarActor));

	SpawnedCharacter->FinishSpawning(MinionTransform);
}
