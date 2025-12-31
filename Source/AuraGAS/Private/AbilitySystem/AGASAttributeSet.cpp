// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AGASGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "AbilitySystem/ExecCalcs/ExecCalc_Damage.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/AGASCombatInterface.h"
#include "Interaction/AGASPlayerInterface.h"
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
	
	// blanket fix for stopping all damage after character death
	// NOTE: for debuff effects (like burn) it still runs the function but just returns
	if (Props.TargetProperties->Character->Implements<UAGASCombatInterface>() && IAGASCombatInterface::Execute_IsDead(Props.TargetProperties->Character)) return;

	// in our case, we consider any GE that has a period/frequency to be a debuff (so all DoT damage)
	const bool bIsDebuff = Data.EffectSpec.Period > 0.f;
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props, bIsDebuff);
	}
	
	if (Data.EvaluatedData.Attribute == GetIncomingForceAttribute())
	{
		HandleKnockback(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPPointsAttribute())
	{
		const float LocalIncomingXPPoints = GetIncomingXPPoints();
		SetIncomingXPPoints(0.f);
		
		if (Props.SourceProperties->AvatarActor->Implements<UAGASPlayerInterface>() && LocalIncomingXPPoints > 0.f)
		{
			// Leveling up is handled in the Player State after this interface function gets executed
			IAGASPlayerInterface::Execute_AddToXPPointsOnPlayerState(Props.SourceProperties->AvatarActor, LocalIncomingXPPoints);
		}
	}
}

void UAGASAttributeSet::MaximizeVitalAttributes()
{
	SetHealthPoints(GetMaxHealthPoints());
	SetManaPoints(GetMaxManaPoints());
}

void UAGASAttributeSet::SendXPPointsEvent(const FEffectPropertiesAdvanced& Props)
{
	if (Props.TargetProperties->AvatarActor->Implements<UAGASCombatInterface>())
	{
		const int32 TargetLevel = IAGASCombatInterface::Execute_GetCharacterLevel(Props.TargetProperties->AvatarActor);
		const ECharacterClass TargetClass = IAGASCombatInterface::Execute_GetCharacterClass(Props.TargetProperties->AvatarActor);
		const int32 XPPointsReward = UAGASAbilitySystemLibrary::GetXPPointsRewarded(Props.TargetProperties->AvatarActor, TargetClass, TargetLevel);

		FGameplayEventData Payload;
		Payload.EventTag = TAG_Attributes_Meta_IncomingXPPoints;
		Payload.EventMagnitude = XPPointsReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceProperties->AvatarActor, TAG_Attributes_Meta_IncomingXPPoints, Payload);
	}
}

void UAGASAttributeSet::HandleIncomingDamage(const FEffectPropertiesAdvanced& Props, const bool bIsDebuff)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage < 0.f) return;
		
	const float NewHealth = GetHealthPoints() - LocalIncomingDamage;
	SetHealthPoints(FMath::Clamp(NewHealth, 0.f, GetMaxHealthPoints()));

	IAGASCombatInterface* CombatInterface = Cast<IAGASCombatInterface>(Props.TargetProperties->AvatarActor);
	
	const bool bFatal = NewHealth <= 0.f;
	if (bFatal)
	{
		if (CombatInterface)
		{
			CombatInterface->Die(UAGASAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
		}
		SendXPPointsEvent(Props);
	}
	else if (!bIsDebuff)
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(TAG_Abilities_HitReact);
		Props.TargetProperties->AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
		
		if (UAGASAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			// Handle debuff
			Debuff(Props);
		}
	}
		
	const bool bCriticalHit = UAGASAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
	const bool bBlocked = UAGASAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
	ShowFloatingText(Props, LocalIncomingDamage, bCriticalHit, bBlocked);
}

void UAGASAttributeSet::HandleKnockback(const FEffectPropertiesAdvanced& Props)
{
	const float LocalIncomingForce = GetIncomingForce();
	SetIncomingForce(0.f);
	if (LocalIncomingForce < 0.f) return;
	
	if (!Props.TargetProperties->Character->Implements<UAGASCombatInterface>()) return;
	
	const FVector DirectionVector = UAGASAbilitySystemLibrary::GetKnockbackDirection(Props.EffectContextHandle);
	const FVector KnockbackForce = DirectionVector * LocalIncomingForce;
	
	// Cancel any ongoing abilities when getting knocked back
	FGameplayTagContainer AbilitiesToCancel;
	AbilitiesToCancel.AddTag(TAG_Abilities_Attack);
	AbilitiesToCancel.AddTag(TAG_Abilities_Summon);
	// TODO: refactor gameplay tags to have a parent tag for ability element and AppendTags for children of elements 
	AbilitiesToCancel.AddTag(TAG_Abilities_Fire_FireBolt);
	AbilitiesToCancel.AddTag(TAG_Abilities_Lightning_Electrocute);
	Props.TargetProperties->AbilitySystemComponent->CancelAbilities(&AbilitiesToCancel);
	
	IAGASCombatInterface::Execute_KnockbackCharacter(Props.TargetProperties->Character, KnockbackForce);
}

void UAGASAttributeSet::Debuff(const FEffectPropertiesAdvanced& Props)
{
	// create a new effect context handle for the dynamic debuff GE
	FGameplayEffectContextHandle DebuffEffectContextHandle = Props.SourceProperties->AbilitySystemComponent->MakeEffectContext();
	DebuffEffectContextHandle.AddSourceObject(Props.SourceProperties->AvatarActor);
	
	// obtain all necessary tags and floats
	const FGameplayTag DamageTypeTag = UAGASAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UAGASAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffFrequency = UAGASAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);
	const float DebuffDuration = UAGASAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	
	// create a new GameplayEffect object
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff %s"), *DamageTypeTag.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
	
	// set its policy and durations
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->bExecutePeriodicEffectOnApplication = false;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	// set stacking policy
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;
	
	// Original way to apply damage. Does not account for resistances and debuffs
	// add a modifier for debuff damage and set it; it is possible to add more modifiers in the same way
	// const int32 Index = Effect->Modifiers.Add(FGameplayModifierInfo());
	// FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
	//
	// ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	// ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	// ModifierInfo.Attribute = GetIncomingDamageAttribute();
	
	// Alternative way using ExecCalc_Damage
	FGameplayEffectExecutionDefinition ExecutionDef;
	ExecutionDef.CalculationClass = UExecCalc_Damage::StaticClass();
	Effect->Executions.Add(ExecutionDef);
	
	// Add the debuff tag to the gameplay effect
	// NOTE: the next few lines of code is the updated way to add tags to "InheritableOwnedTagsContainer" from previous versions
	// that variable has depreciated, so this is the new way (next 4 lines tbe)
	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.AddTag(AGASGameplayTags::GetDamageTypeToDebuffMap()[DamageTypeTag]);
	Component.SetAndApplyTargetTagChanges(TagContainer);
	
	// create a new GameplayEffectSpec, set its damage type and apply that effect
	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, DebuffEffectContextHandle, 1.f))
	{
		// set the damage value for ExecCalc_Damage
		MutableSpec->SetSetByCallerMagnitude(DamageTypeTag, DebuffDamage);
		
		FAGASGameplayEffectContext* AGASDebuffEffectContext = static_cast<FAGASGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageTypeTag));
		AGASDebuffEffectContext->SetDamageType(DebuffDamageType);
		
		Props.TargetProperties->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*MutableSpec);
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

