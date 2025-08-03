// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "AGASAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class AURAGAS_API AAGASAIController : public AAIController
{
	GENERATED_BODY()

public:
	
	AAGASAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
};
