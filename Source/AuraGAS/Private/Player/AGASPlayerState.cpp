// Copyright Dustin Carmelo


#include "Player/AGASPlayerState.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAGASPlayerState::AAGASPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAGASAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void AAGASPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Level);
}

UAbilitySystemComponent* AAGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAGASPlayerState::OnRep_Level(int32 OldLevel)
{
	
}
