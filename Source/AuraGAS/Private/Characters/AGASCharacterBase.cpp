// Copyright Dustin Carmelo


#include "Characters/AGASCharacterBase.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "AuraGAS/AuraGAS.h"


AAGASCharacterBase::AAGASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");
}

UAbilitySystemComponent* AAGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AAGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAGASCharacterBase::GetCombatSocketLocation()
{
	check(WeaponMesh)
	return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
}

void AAGASCharacterBase::InitializeAbilityActorInfo()
{
	
}

void AAGASCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	/**
	 * NOTE: We use GetAbilitySystemComponent() in this function since at the base level, there is no ASC yet to be
	 * established. We have only established it in AGASPlayerState/Character and AGASEnemy
	 */
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, GetAbilitySystemComponent());
}

void AAGASCharacterBase::InitializeDefaultStats() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAGASCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;

	AbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

