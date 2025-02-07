// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAttributeSet.h"

#include "Net/UnrealNetwork.h"

UAGASAttributeSet::UAGASAttributeSet()
{
	InitHealthPoints(100.f);
	InitMaxHealthPoints(100.f);
	InitManaPoints(50.f);
	InitMaxManaPoints(50.f);
}

void UAGASAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealthPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxManaPoints, COND_None, REPNOTIFY_Always);
}

void UAGASAttributeSet::OnRep_HealthPoints(const FGameplayAttributeData& OldHealthPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthPoints, OldHealthPoints)
}

void UAGASAttributeSet::OnRep_MaxHealthPoints(const FGameplayAttributeData& OldMaxHealthPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealthPoints, OldMaxHealthPoints)
}

void UAGASAttributeSet::OnRep_ManaPoints(const FGameplayAttributeData& OldManaPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ManaPoints, OldManaPoints)
}

void UAGASAttributeSet::OnRep_MaxManaPoints(const FGameplayAttributeData& OldMaxManaPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxManaPoints, OldMaxManaPoints)
}

