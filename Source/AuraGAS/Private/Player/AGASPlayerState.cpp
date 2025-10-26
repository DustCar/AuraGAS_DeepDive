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
	DOREPLIFETIME(ThisClass, XPPoints);
}

UAbilitySystemComponent* AAGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAGASPlayerState::SetLevel(const int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChangedSignature.Broadcast(Level);
}

void AAGASPlayerState::SetXPPoints(const int32 NewXPPoints)
{
	XPPoints = NewXPPoints;
	OnXPPointsChangedSignature.Broadcast(XPPoints);
}

void AAGASPlayerState::AddToLevel(const int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedSignature.Broadcast(Level);
}

void AAGASPlayerState::AddToXPPoints(const int32 InXPPoints)
{
	XPPoints += InXPPoints;
	OnXPPointsChangedSignature.Broadcast(XPPoints);
}

void AAGASPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedSignature.Broadcast(Level);
}

void AAGASPlayerState::OnRep_XPPoints(int32 OldXPPoints)
{
	OnXPPointsChangedSignature.Broadcast(XPPoints);
}
