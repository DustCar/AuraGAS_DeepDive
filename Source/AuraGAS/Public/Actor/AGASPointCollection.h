// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGASPointCollection.generated.h"

#define CREATE_AND_SETUP_PT(Pt_X) \
	Pt_X = CreateDefaultSubobject<USceneComponent>(TEXT(#Pt_X)); \
	Points.Add(Pt_X); \
	Pt_X->SetupAttachment(GetRootComponent());

UCLASS()
class AURAGAS_API AAGASPointCollection : public AActor
{
	GENERATED_BODY()

public:
	AAGASPointCollection();
	
	UFUNCTION(BlueprintPure)
	TArray<FTransform> GetPointsOnGeometry(int32 PointCount, float YawOverride = 0.f);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<TObjectPtr<USceneComponent>> Points;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_Root;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_1;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_2;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_3;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_4;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_5;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_6;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_7;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_8;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_9;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_10;
};
