// Copyright Dustin Carmelo


#include "Actor/AGASEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGAS/AuraGAS.h"


AAGASEffectActor::AAGASEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAGASEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAGASEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetAbilitySystemComponent == nullptr) return;

	check(GameplayEffectClass);
	
	// note: when it comes to handle variable names, other projects generally omit the "handle" portion of the name
	FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);

	const bool bIsInfinite = EffectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy ==  EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		auto& AffectedActor = ActiveInfiniteEffects.FindOrAdd(TargetActor->GetUniqueID());
		auto& InstigatorActorEffectsArray = AffectedActor.FindOrAdd(GetUniqueID());
		InstigatorActorEffectsArray.Add(ActiveEffectHandle);
	}

	if (ActorDestructionPolicy == EActorDestructionPolicy::DestroyOnEffectApplication && InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::DoNotApply)
	{
		Destroy();
	}
}

void AAGASEffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(ACTOR_TAG_ENEMY) && !bApplyEffectsToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto& InstantEffect : InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, InstantEffect);
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto& DurationEffect : DurationGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, DurationEffect);
		}
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto& InfiniteEffect : InfiniteGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, InfiniteEffect);
		}
	}
}

void AAGASEffectActor::RemoveInfiniteEffectsFromActor(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!IsValid(TargetASC)) return;
		
	const auto UID = TargetActor->GetUniqueID();
	if (!ActiveInfiniteEffects.Contains(UID)) return;
	if (!ActiveInfiniteEffects[UID].Contains(GetUniqueID())) return;

	auto& ActorsAppliedEffects = ActiveInfiniteEffects[UID][GetUniqueID()];
	for (const auto& AppliedEffect : ActorsAppliedEffects)
	{
		if (!TargetASC->RemoveActiveGameplayEffect(AppliedEffect, 1)) return;
	}

	ActiveInfiniteEffects[UID].Remove(GetUniqueID());
	if (ActiveInfiniteEffects[UID].IsEmpty())
	{
		ActiveInfiniteEffects.Remove(UID);
	}
}

void AAGASEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(ACTOR_TAG_ENEMY) && !bApplyEffectsToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto& InstantEffect : InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, InstantEffect);
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto& DurationEffect : DurationGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, DurationEffect);
		}
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto& InfiniteEffect : InfiniteGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, InfiniteEffect);
		}
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		RemoveInfiniteEffectsFromActor(TargetActor);
	}
}

