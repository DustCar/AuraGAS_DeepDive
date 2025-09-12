// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASGA_CastSummon.h"

#include "Kismet/KismetSystemLibrary.h"

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
		const FVector ChosenSpawnLocation = OwnerLocation + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		SpawnLocations.Add(ChosenSpawnLocation);
		
		DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 10.f, 12, FColor::Cyan, false, 3.f);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			OwnerLocation,
			OwnerLocation + Direction * MaxSpawnDistance,
			4.f,
			FLinearColor::Green,
			3.f
		);
		
		DrawDebugSphere(GetWorld(), OwnerLocation + Direction * MinSpawnDistance, 15.f, 12, FColor::Red, false, 3.f);
		DrawDebugSphere(GetWorld(), OwnerLocation + Direction * MaxSpawnDistance, 15.f, 12, FColor::Blue, false, 3.f);
	}

	return SpawnLocations;
}
