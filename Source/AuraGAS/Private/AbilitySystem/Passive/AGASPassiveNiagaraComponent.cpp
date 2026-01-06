// Copyright Dustin Carmelo


#include "AbilitySystem/Passive/AGASPassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "Interaction/AGASCombatInterface.h"


UAGASPassiveNiagaraComponent::UAGASPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UAGASPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (UAGASAbilitySystemComponent* AGASASC = Cast<UAGASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AGASASC->ActivatePassiveEffect.AddUObject(this, &ThisClass::OnPassiveActivate);
	}
	else if (IAGASCombatInterface* CombatInterface = Cast<IAGASCombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (UAGASAbilitySystemComponent* AGASASC = Cast<UAGASAbilitySystemComponent>(ASC))
			{
				AGASASC->ActivatePassiveEffect.AddUObject(this, &ThisClass::OnPassiveActivate);
			}
		});
	}
}

void UAGASPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}


