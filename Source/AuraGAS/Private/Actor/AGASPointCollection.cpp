// Copyright Dustin Carmelo


#include "Actor/AGASPointCollection.h"

#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "AuraGAS/AGASLogChannels.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


AAGASPointCollection::AAGASPointCollection()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Pt_Root = CreateDefaultSubobject<USceneComponent>("Pt_Root");
	Points.Add(Pt_Root);
	SetRootComponent(Pt_Root);
	
	CREATE_AND_SETUP_PT(Pt_1);
	CREATE_AND_SETUP_PT(Pt_2);
	CREATE_AND_SETUP_PT(Pt_3);
	CREATE_AND_SETUP_PT(Pt_4);
	CREATE_AND_SETUP_PT(Pt_5);
	CREATE_AND_SETUP_PT(Pt_6);
	CREATE_AND_SETUP_PT(Pt_7);
	CREATE_AND_SETUP_PT(Pt_8);
	CREATE_AND_SETUP_PT(Pt_9);
	CREATE_AND_SETUP_PT(Pt_10);
}

TArray<FTransform> AAGASPointCollection::GetPointsOnGeometry(int32 PointCount, float YawOverride)
{
	checkf(Points.Num() > 0, TEXT("%hs: The number of ImmutablePts must be greater than 0."), __FUNCTION__);
	
	if (PointCount < 1)
	{
		UE_LOG(LogAGAS, Warning, TEXT("%hs: Called with invalid PointCount (%i), PointCount must be greater than 0."), __FUNCTION__, PointCount);
		return TArray<FTransform>();
	}
	
	if (PointCount > Points.Num())
	{
		UE_LOG(LogAGAS, Warning, TEXT("%hs: PointCount (%i) exceeds the max number of ImmutablePts, setting PointCount to max available (%i)."), __FUNCTION__, PointCount, Points.Num());
		PointCount = Points.Num();
	}
	
	TArray<FTransform> PointsAtLocation;
	
	for (TObjectPtr<USceneComponent> Point : Points)
	{
		FTransform PointTransform = Point->GetComponentTransform();
		
		if (Point != Pt_Root)
		{
			// obtain the difference vector from current point and root; this will determine the rotation of the point based on location from root and YawOverride
			FVector RootToPointVector = PointTransform.GetLocation() - GetActorLocation();
			RootToPointVector = RootToPointVector.RotateAngleAxis(YawOverride, FVector::UpVector);
			PointTransform.SetLocation(GetActorLocation() + RootToPointVector);
		}
		
		// setup params for line trace to floor
		FHitResult HitResult;
		const FVector RaisedLocation = FVector(PointTransform.GetLocation().X, PointTransform.GetLocation().Y, PointTransform.GetLocation().Z + 250.f);
		const FVector LoweredLocation = FVector(PointTransform.GetLocation().X, PointTransform.GetLocation().Y, PointTransform.GetLocation().Z - 250.f);
		
		// get the closest actors to ignore in the line trace
		TArray<AActor*> AliveActorsWithinRadius;
		UAGASAbilitySystemLibrary::GetLivePlayersWithinRadius(this, AliveActorsWithinRadius, TArray<AActor*>(), 1500.f, GetActorLocation());
		
		UKismetSystemLibrary::LineTraceSingle(this, RaisedLocation, LoweredLocation, TraceTypeQuery1, false, AliveActorsWithinRadius, EDrawDebugTrace::None, HitResult, true);
		
		// adjust the z of the point's location to be the one of the floor (hit result) and add to array
		if (HitResult.bBlockingHit)
		{
			const FVector ZAdjustedPointLocation = FVector(PointTransform.GetLocation().X, PointTransform.GetLocation().Y, HitResult.ImpactPoint.Z);
			PointTransform.SetLocation(ZAdjustedPointLocation);
			
			PointTransform.SetRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal).Quaternion());
			
			PointsAtLocation.Emplace(PointTransform);
			if (PointsAtLocation.Num() == PointCount) break;
		}
	}
	
	return PointsAtLocation;
}

void AAGASPointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}
