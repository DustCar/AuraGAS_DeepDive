// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AGASPassiveNiagaraComponent.generated.h"


class UAGASAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAS_API UAGASPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	
	UAGASPassiveNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;
	
protected:
	virtual void BeginPlay() override;
	
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
	
private:
	void ActivateIfEquipped(UAGASAbilitySystemComponent* AGASASC);
};
