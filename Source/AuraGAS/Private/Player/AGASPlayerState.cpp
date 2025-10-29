// Copyright Dustin Carmelo


#include "Player/AGASPlayerState.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "AbilitySystem/Data/AGASLevelUpInfo.h"
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

void AAGASPlayerState::SetAttributePoints(const int32 NewAttributePoints)
{
	AttributePoints = NewAttributePoints;
	OnAttributePointsChangedSignature.Broadcast(AttributePoints);
}

void AAGASPlayerState::SetSpellPoints(const int32 NewSpellPoints)
{
	SpellPoints = NewSpellPoints;
	OnSpellPointsChangedSignature.Broadcast(SpellPoints);
}

void AAGASPlayerState::AddToLevel(const int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedSignature.Broadcast(Level);
}

void AAGASPlayerState::AddToXPPoints(const int32 InXPPoints)
{
	XPPoints += InXPPoints;

	const int32 NewLevel = LevelUpInfo->FindLevelForXP(XPPoints);
	const int32 LevelUpCount = NewLevel - Level;
	if (LevelUpCount > 0)
	{
		for (int32 GainedLevel = Level; GainedLevel < NewLevel; ++GainedLevel)
		{
			const int32 GainedAttributePoints = LevelUpInfo->LevelUpInformation[GainedLevel].AttributePointsReward;
			AddToAttributePoints(GainedAttributePoints);
			const int32 GainedSpellPoints = LevelUpInfo->LevelUpInformation[GainedLevel].SpellPointsReward;
			AddToSpellPoints(GainedSpellPoints);
		}
		AddToLevel(LevelUpCount);
		AttributeSet->MaximizeVitalAttributes();
	}
	OnXPPointsChangedSignature.Broadcast(XPPoints);
}

void AAGASPlayerState::AddToAttributePoints(const int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedSignature.Broadcast(AttributePoints);
}

void AAGASPlayerState::AddToSpellPoints(const int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointsChangedSignature.Broadcast(SpellPoints);
}

void AAGASPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedSignature.Broadcast(Level);
}

void AAGASPlayerState::OnRep_XPPoints(int32 OldXPPoints)
{
	OnXPPointsChangedSignature.Broadcast(XPPoints);
}

void AAGASPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedSignature.Broadcast(AttributePoints);
}

void AAGASPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedSignature.Broadcast(SpellPoints);
}
