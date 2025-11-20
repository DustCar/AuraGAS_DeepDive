// Copyright Dustin Carmelo


#include "Characters/AGASCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AuraGAS/AuraGAS.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AGASPlayerState.h"


AAGASCharacter::AAGASCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(RootComponent);
	LevelUpNiagaraComponent->bAutoActivate = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->bUsePawnControlRotation = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void AAGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Emplace(ACTOR_TAG_PLAYER);
}

void AAGASCharacter::InitializeAbilityActorInfo()
{
	AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);

	AbilitySystemComponent = AGASPlayerState->GetAGASAbilitySystemComponent();
	AttributeSet = AGASPlayerState->GetAttributeSet();
	AbilitySystemComponent->InitAbilityActorInfo(AGASPlayerState, this);
	AbilitySystemComponent->AbilityActorInfoSet();

	AGASPlayerState->OnLevelChangedSignature.AddLambda([this] (const int32 NewValue)
	{
		MulticastLevelUpParticles();
	});

	// Can be called on server side only since attributes are replicated, however, it is okay to call on server and clients
	InitializeDefaultStats();
}

void AAGASCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = CameraComponent->GetComponentLocation();
		const FVector ActorLocation = GetActorLocation();
		const FVector CameraForwardVector = ActorLocation - CameraLocation;
		LevelUpNiagaraComponent->SetWorldRotation(CameraForwardVector.Rotation());
		LevelUpNiagaraComponent->Activate(true);
	}
}

void AAGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize Ability Actor Info for server
	InitializeAbilityActorInfo();
	AddCharacterAbilities();
}

void AAGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize Ability Actor Info for client
	InitializeAbilityActorInfo();
}

int32 AAGASCharacter::GetCharacterLevel_Implementation()
{
	const AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);
	
	return AGASPlayerState->GetPlayerLevel();
}

void AAGASCharacter::AddToXPPointsOnPlayerState_Implementation(int32 InXPPoints)
{
	AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);

	AGASPlayerState->AddToXPPoints(InXPPoints);
}

void AAGASCharacter::AddToAttributePointsOnPlayerState_Implementation(int32 InAttributePoints)
{
	AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);

	AGASPlayerState->AddToAttributePoints(InAttributePoints);
}

int32 AAGASCharacter::GetAttributePointsOnPlayerState_Implementation()
{
	AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);

	return AGASPlayerState->GetAttributePoints();
}

/**
 * Old function that was used in AGASCharacterBase.h that applies a GameplayEffectSpec to the character
 * with said GameplayEffectClass.
 *
 * Updated function for Character is ApplyGameplayEffectSpecToSelf that takes in a GameplayEffectContextHandle
 * param so that a single context handle can be reused and is only used on the Player Character since enemies
 * use the AGASAbilitySystemLibrary to initialize default attributes
 */
/*
void AAGASCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	
	// NOTE: We use GetAbilitySystemComponent() in this function since at the base level, there is no ASC yet to be
	// established. We have only established it in AGASPlayerState/Character and AGASEnemy
	 
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, GetAbilitySystemComponent());
}
*/

void AAGASCharacter::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass,
	const FGameplayEffectContextHandle& EffectContextHandle, const float Level) const
{
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void AAGASCharacter::InitializeDefaultStats() const
{
	// Updated to use the check macro within here instead of the old ApplyEffectToSelf since Character would have an ASC
	// also makes the function much easier to read and reduces redundancy
	check(IsValid(AbilitySystemComponent));
	check(DefaultPrimaryAttributes);
	check(DefaultSecondaryAttributes);
	check(DefaultVitalAttributes);
	
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	ApplyGameplayEffectToSelf(DefaultPrimaryAttributes, ContextHandle);
	ApplyGameplayEffectToSelf(DefaultSecondaryAttributes, ContextHandle);
	ApplyGameplayEffectToSelf(DefaultVitalAttributes, ContextHandle);
}

void AAGASCharacter::AddCharacterAbilities()
{
	if (!HasAuthority()) return;

	AbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	AbilitySystemComponent->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}


