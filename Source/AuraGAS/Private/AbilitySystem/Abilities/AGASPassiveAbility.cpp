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

FString UAGASPassiveAbility::FormatPassiveAbilityDescription(int32 Level, const FString& Description)
{
	FString NewDescription = Description;
	FStringFormatNamedArguments NamesToValues;
	
	NamesToValues.Add(TEXT("_Percent0"), FMath::TruncToInt32(GetPercentAtLevel(Level)));
	NamesToValues.Add(TEXT("_Percent1"), FMath::TruncToInt32(GetPercentAtLevel(Level + 1)));
	NamesToValues.Add(TEXT("_LineBreak"), "\n");
	
	NewDescription = FString::Format(*Description, NamesToValues);
	
	return NewDescription;
}

void UAGASPassiveAbility::ReceivedDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}
