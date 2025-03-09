// Copyright Dustin Carmelo


#include "Characters/AGASCharacterBase.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"


AAGASCharacterBase::AAGASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AAGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGASCharacterBase::InitializeAbilityActorInfo()
{
	
}

void AAGASCharacterBase::InitializeMainStats() const
{
	/**
	 * NOTE: We use GetAbilitySystemComponent() in this function since at the base level, there is no ASC yet to be
	 * established. We have only established it in AGASPlayerState/Character and AGASEnemy
	 */
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultMainStats);
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultMainStats, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, GetAbilitySystemComponent());
}

