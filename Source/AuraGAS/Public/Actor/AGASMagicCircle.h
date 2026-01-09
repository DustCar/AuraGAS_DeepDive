// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGASMagicCircle.generated.h"

UCLASS()
class AURAGAS_API AAGASMagicCircle : public AActor
{
	GENERATED_BODY()

public:
	AAGASMagicCircle();
	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:
	virtual void BeginPlay() override;

};
