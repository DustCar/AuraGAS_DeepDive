// Copyright Dustin Carmelo


#include "AbilitySystem/Passive/AGASPassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AGASGameplayTags.h"
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
		ActivateIfEquipped(AGASASC);
	}
	else if (IAGASCombatInterface* CombatInterface = Cast<IAGASCombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (UAGASAbilitySystemComponent* AGASASC = Cast<UAGASAbilitySystemComponent>(ASC))
			{
				AGASASC->ActivatePassiveEffect.AddUObject(this, &ThisClass::OnPassiveActivate);
				ActivateIfEquipped(AGASASC);
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

void UAGASPassiveNiagaraComponent::ActivateIfEquipped(UAGASAbilitySystemComponent* AGASASC)
{
	if (AGASASC->bStartupAbilitiesGiven)
	{
		FGameplayAbilitySpec* AbilitySpec = AGASASC->GetSpecFromAbilityTag(PassiveSpellTag);
		if (AGASASC->GetStatusTagFromAbilityTag(PassiveSpellTag).MatchesTagExact(TAG_Abilities_Status_Equipped))
		{
			Activate();
		}
	}
}


