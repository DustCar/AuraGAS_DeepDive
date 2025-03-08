// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAGASAttributeSet::UAGASAttributeSet()
{
	InitHealthPoints(50.f);
	InitMaxHealthPoints(100.f);
	InitManaPoints(25.f);
	InitMaxManaPoints(50.f);
}

// Needed for Replication
void UAGASAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealthPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxManaPoints, COND_None, REPNOTIFY_Always);
}

void UAGASAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthPointsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealthPoints());
	}
	if (Attribute == GetManaPointsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxManaPoints());
	}
}

// Saves information of whom the GameplayEffect affects and from whom.
void UAGASAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectPropertiesAdvanced& Props)
{
	// Source = cause of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContext = Data.EffectSpec.GetContext();
	Props.SourceProperties->AbilitySystemComponent = Props.EffectContext.GetOriginalInstigatorAbilitySystemComponent();

	if (!IsValid(Props.SourceProperties->AbilitySystemComponent)) return;
	
	// UAbilitySystemComponent::GetAvatarActor() already checks AbilityActorInfo, so if the line runs then it should
	// be valid
	Props.SourceProperties->AvatarActor = Props.SourceProperties->AbilitySystemComponent->GetAvatarActor();
	if (!IsValid(Props.SourceProperties->AvatarActor)) return;
	
	Props.SourceProperties->Controller = Props.SourceProperties->AbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
	if (Props.SourceProperties->Controller == nullptr && Props.SourceProperties->AvatarActor != nullptr)
	{
		if (const APawn* Pawn = Cast<APawn>(Props.SourceProperties->AvatarActor))
		{
			Props.SourceProperties->Controller = Pawn->GetController();
		}
	}
	if (Props.SourceProperties->Controller)
	{
		Props.SourceProperties->Character = Cast<ACharacter>(Props.SourceProperties->Controller->GetPawn());
	}

	Props.TargetProperties->AvatarActor = Data.Target.GetAvatarActor();
	if (!IsValid(Props.TargetProperties->AvatarActor)) return;

	Props.TargetProperties->Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
	Props.TargetProperties->Character = Cast<ACharacter>(Props.TargetProperties->AvatarActor);
	Props.TargetProperties->AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetProperties->AvatarActor);
}

void UAGASAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectPropertiesAdvanced Props;
	SetEffectProperties(Data, Props);
	
}

//~ Replication needed BEGIN
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
//~ Replication needed END
