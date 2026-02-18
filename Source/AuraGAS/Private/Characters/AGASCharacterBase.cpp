// Copyright Dustin Carmelo


#include "Characters/AGASCharacterBase.h"

#include "AGASGameplayTags.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/Debuff/AGASDebuffNiagaraComponent.h"
#include "AbilitySystem/Passive/AGASPassiveNiagaraComponent.h"
#include "AuraGAS/AuraGAS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AAGASCharacterBase::AAGASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	if (IsValid(WeaponMesh))
	{
		WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");
	
	BurnDebuffComponent = CreateDefaultSubobject<UAGASDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = TAG_Debuff_Burn;
	
	StunDebuffComponent = CreateDefaultSubobject<UAGASDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = TAG_Debuff_Stun;
	
	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachComponent");
	EffectAttachComponent->SetupAttachment(GetRootComponent());
	EffectAttachComponent->SetUsingAbsoluteRotation(true);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
	
	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UAGASPassiveNiagaraComponent>("HaloOfProtectionComponent");
	HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);
	
	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UAGASPassiveNiagaraComponent>("LifeSiphonNiagaraComponent");
	LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	
	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UAGASPassiveNiagaraComponent>("ManaSiphonNiagaraComponent");
	ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
}

void AAGASCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAGASCharacterBase, bIsStunned);
	DOREPLIFETIME(AAGASCharacterBase, bIsBurned);
	DOREPLIFETIME(AAGASCharacterBase, bIsBeingShocked);
}

UAbilitySystemComponent* AAGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAGASCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAGASCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void AAGASCharacterBase::BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsBurned = NewCount > 0;
}

void AAGASCharacterBase::OnRep_IsStunned()
{
}

void AAGASCharacterBase::OnRep_IsBurned()
{
}

void AAGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGASCharacterBase::Die(const FVector& DeathImpulse)
{
	// If character was summoned, decrement their summoner's minion count
	if (bWasSummoned && IsValid(GetInstigator()))
	{
		// Had to use Instigator since that returned the actual character that spawned the minion
		// Owner would return the AIController instead
		APawn* OwnerPawn = GetInstigator();
		Execute_AddToMinionCount(OwnerPawn, -1);
	}
	if (IsValid(WeaponMesh))
	{
		WeaponMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}
	
	MulticastHandleDeath(DeathImpulse);
}

void AAGASCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	}
	// Implementing ragdoll physics for the weapon and the character mesh
	if (IsValid(WeaponMesh->GetSkeletalMeshAsset()))
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		WeaponMesh->AddImpulse(DeathImpulse * 2);
	}

	const float CharMeshMass = GetMesh()->GetMass();
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse * CharMeshMass);
	
	// Set capsule component collision response to WorldStatic block only to avoid an invisible actor and clipping through
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// Runs a dissolve animation
	Dissolve();
	bDead = true;
	OnDeath.Broadcast(this);
}

FVector AAGASCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag)
{
	if (CombatSocketTag.MatchesTagExact(TAG_CombatSocket_Weapon) && IsValid(WeaponMesh))
	{
		return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
	}
	if (CombatSocketTag.MatchesTagExact(TAG_CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (CombatSocketTag.MatchesTagExact(TAG_CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (CombatSocketTag.MatchesTagExact(TAG_CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}

	return FVector();
}

bool AAGASCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAGASCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAGASCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

FTaggedMontage AAGASCharacterBase::GetAttackMontageRandom_Implementation()
{
	if (AttackMontages.Num() == 0)
	{
		return FTaggedMontage();
	}

	// If there is only one element in the array, then return that element
	if (AttackMontages.Num() == 1)
	{
		return AttackMontages[0];
	}
	// return a random attack montage if more than one
	return AttackMontages[FMath::RandRange(0, AttackMontages.Num() - 1)];
}

UNiagaraSystem* AAGASCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

USoundBase* AAGASCharacterBase::GetImpactSoundByMontageTag_Implementation(const FGameplayTag& InMontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(InMontageTag))
		{
			return TaggedMontage.ImpactSound;
		}
	}
	return nullptr;
}

int32 AAGASCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAGASCharacterBase::AddToMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass AAGASCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

void AAGASCharacterBase::KnockbackCharacter_Implementation(const FVector& KnockbackForce)
{
	// could add additional logic to make knockback feel better but for now we'll just refactor our knockback to use
	// a combat interface function rather than just calling it straight up in AttributeSet
	LaunchCharacter(KnockbackForce, false, false);
	
}

USkeletalMeshComponent* AAGASCharacterBase::GetWeapon_Implementation()
{
	if (IsValid(WeaponMesh))
	{
		return WeaponMesh;
	}
	
	return nullptr;
}

void AAGASCharacterBase::SetIsBeingShocked_Implementation(bool bInIsBeingShocked)
{
	bIsBeingShocked = bInIsBeingShocked;
	OnBeingShocked.ExecuteIfBound(bInIsBeingShocked);
}

bool AAGASCharacterBase::IsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

FOnASCRegistered& AAGASCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

FOnDeath& AAGASCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

void AAGASCharacterBase::SetWasSummoned(bool bInWasSummoned)
{
	bWasSummoned = bInWasSummoned;
}


void AAGASCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* WeaponDynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		WeaponMesh->SetMaterial(0, WeaponDynamicMatInst);
		StartWeaponDissolveTimeline(WeaponDynamicMatInst);
	}
}

void AAGASCharacterBase::InitializeAbilityActorInfo()
{
	
}

void AAGASCharacterBase::AddCharacterAbilities()
{
	
}

