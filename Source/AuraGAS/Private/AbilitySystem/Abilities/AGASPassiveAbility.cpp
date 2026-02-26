// Copyright Dustin Carmelo


#include "AbilitySystem/Abilities/AGASPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"

void UAGASPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAGASAbilitySystemComponent* AGASASC = Cast<UAGASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary:: GetAbilitySystemComponent(GetAvatarActorFromActorInfo()));
	if (AGASASC && !AGASASC->DeactivatePassiveAbility.IsBoundToObject(this))
	{
		AGASASC->DeactivatePassiveAbility.AddUObject(this, &ThisClass::ReceivedDeactivate);
	}
}

float UAGASPassiveAbility::GetPercentAtLevel(int32 Level) const
{
	if (PercentEffective.IsValid())
	{
		return PercentEffective.GetValueAtLevel(Level);
	}
	
	return 0.f;
}

void UAGASPassiveAbility::ReceivedDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}
