// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AGASGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/AGASCombatInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AGASPlayerController.h"

UAGASAttributeSet::UAGASAttributeSet()
{
	
}

// Needed for Replication
void UAGASAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/**
	 * Vital Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaPoints, COND_None, REPNOTIFY_Always);

	/**
	 * Primary Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Vigor, COND_None, REPNOTIFY_Always);

	/**
	 * Secondary Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealthPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxManaPoints, COND_None, REPNOTIFY_Always);

	/**
	 * Resistance Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, PhysicalResistance, COND_None, REPNOTIFY_Always);
	
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

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceProperties->AbilitySystemComponent = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

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

void UAGASAttributeSet::ShowFloatingText(const FEffectPropertiesAdvanced& Props, const float Damage, const bool bCriticalHit, const bool bBlockedHit) const
{
	if (Props.SourceProperties->Character != Props.TargetProperties->Character)
	{
		// shows damage done by player
		if (auto AGASPC = Cast<AAGASPlayerController>(Props.SourceProperties->Character->Controller))
		{
			AGASPC->ShowDamageNumber(Damage, Props.TargetProperties->Character, bCriticalHit, bBlockedHit);
		}

		// shows damage done by enemy
		if (auto AGASPC = Cast<AAGASPlayerController>(Props.TargetProperties->Character->Controller))
		{
			AGASPC->ShowDamageNumber(Damage, Props.TargetProperties->Character, bCriticalHit, bBlockedHit);
		}
		
		/* Separate code in case we want damage to display to all clients */
		// for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		// {
		// 	if (auto AGASPC = Cast<AAGASPlayerController>(It->Get()))
		// 	{
		// 		AGASPC->ShowDamageNumber(Damage, Props.TargetProperties->Character);
		// 	}
		// }
	}
}

void UAGASAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectPropertiesAdvanced Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage < 0.f) return;
		
		const float NewHealth = GetHealthPoints() - LocalIncomingDamage;
        SetHealthPoints(FMath::Clamp(NewHealth, 0.f, GetMaxHealthPoints()));

		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			
			IAGASCombatInterface* CombatInterface = Cast<IAGASCombatInterface>(Props.TargetProperties->AvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die();
			}
		}
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(TAG_Effects_HitReact);
			Props.TargetProperties->AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
		}
		
		const bool bCriticalHit = UAGASAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		const bool bBlocked = UAGASAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bCriticalHit, bBlocked);
	}
}

void UAGASAttributeSet::OnRep_HealthPoints(const FGameplayAttributeData& OldHealthPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthPoints, OldHealthPoints);
}

void UAGASAttributeSet::OnRep_ManaPoints(const FGameplayAttributeData& OldManaPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ManaPoints, OldManaPoints);
}

void UAGASAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Strength, OldStrength);
}

void UAGASAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Intelligence, OldIntelligence);
}

void UAGASAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Resilience, OldResilience);
}

void UAGASAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Vigor, OldVigor);
}

void UAGASAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Armor, OldArmor);
}
    
void UAGASAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ArmorPenetration, OldArmorPenetration);
}
    
void UAGASAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, BlockChance, OldBlockChance);
}
    
void UAGASAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitChance, OldCriticalHitChance);
}
    
void UAGASAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitDamage, OldCriticalHitDamage);
}
    
void UAGASAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitResistance, OldCriticalHitResistance);
}
    
void UAGASAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthRegeneration, OldHealthRegeneration);
}
    
void UAGASAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ManaRegeneration, OldManaRegeneration);
}

void UAGASAttributeSet::OnRep_MaxHealthPoints(const FGameplayAttributeData& OldMaxHealthPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealthPoints, OldMaxHealthPoints);
}

void UAGASAttributeSet::OnRep_MaxManaPoints(const FGameplayAttributeData& OldMaxManaPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxManaPoints, OldMaxManaPoints);
}

void UAGASAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, FireResistance, OldFireResistance);
}

void UAGASAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, LightningResistance, OldLightningResistance);
}

void UAGASAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ArcaneResistance, OldArcaneResistance);
}

void UAGASAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, PhysicalResistance, OldPhysicalResistance);
}
