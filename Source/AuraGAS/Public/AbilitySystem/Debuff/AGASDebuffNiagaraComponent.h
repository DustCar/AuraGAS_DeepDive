// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AGASDebuffNiagaraComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAS_API UAGASDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UAGASDebuffNiagaraComponent();
	
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;
	
protected:
	virtual void BeginPlay() override;
	
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
