// Copyright Dustin Carmelo


#include "Characters/AGASCharacter.h"

#include "AbilitySystemComponent.h"
#include "AGASGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "AuraGAS/AuraGAS.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/AGASGameInstance.h"
#include "Game/AGASGameModeBase.h"
#include "Game/AGASLoadMenuSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AGASPlayerController.h"
#include "Player/AGASPlayerState.h"


AAGASCharacter::AAGASCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Target, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Target, ECR_Ignore);

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

void AAGASCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	// NOTE: We need to call DisableInput() on the Actor we need to disable input for, i.e. this Pawn's controller
	// if we call it directly from this Pawn, it won't work
	// Additional Notes: We can't just use DisableInput() like this, we need to use PlayerController->DisableInput() 
	// because the PlayerController is the Actor class that holds all of our Inputs
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
	}
	
	Super::MulticastHandleDeath_Implementation(DeathImpulse);
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
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	
	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_Debuff_Burn, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::BurnTagChanged);

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

void AAGASCharacter::OnRep_IsStunned()
{
	// TODO: might have to refactor when updating Debuff GEs.
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(TAG_Debuff_Stun);
		BlockedTags.AddTag(TAG_Player_Block_CursorTrace);
		BlockedTags.AddTag(TAG_Player_Block_InputHeld);
		BlockedTags.AddTag(TAG_Player_Block_InputPressed);
		BlockedTags.AddTag(TAG_Player_Block_InputReleased);
		
		if (bIsStunned)
		{
			AbilitySystemComponent->AddLooseGameplayTags(BlockedTags);
			
		}
		else
		{
			AbilitySystemComponent->RemoveLooseGameplayTags(BlockedTags);
		}
		
	}
}

void AAGASCharacter::OnRep_IsBurned()
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer BurnTags;
		BurnTags.AddTag(TAG_Debuff_Burn);
		
		if (bIsBurned)
		{
			AbilitySystemComponent->AddLooseGameplayTags(BurnTags);
			
		}
		else
		{
			AbilitySystemComponent->RemoveLooseGameplayTags(BurnTags);
		}
	}
}

int32 AAGASCharacter::GetCharacterLevel_Implementation()
{
	const AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);
	
	return AGASPlayerState->GetPlayerLevel();
}

void AAGASCharacter::KnockbackCharacter_Implementation(const FVector& KnockbackForce)
{
	Super::KnockbackCharacter_Implementation(KnockbackForce);
	GetCharacterMovement()->StopMovementImmediately();
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

void AAGASCharacter::AddToSpellPointsOnPlayerState_Implementation(int32 InSpellPoints)
{
	AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);

	AGASPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AAGASCharacter::GetAttributePointsOnPlayerState_Implementation()
{
	AAGASPlayerState* AGASPlayerState = GetPlayerState<AAGASPlayerState>();
	check(AGASPlayerState);

	return AGASPlayerState->GetAttributePoints();
}

void AAGASCharacter::ShowMagicCircleOnPlayerController_Implementation(float Radius, UMaterialInterface* DecalMaterial)
{
	if (!IsLocallyControlled()) return;
	
	AAGASPlayerController* AGASPlayerController = GetController<AAGASPlayerController>();
	check(AGASPlayerController);
	
	AGASPlayerController->ShowMagicCircle(Radius, DecalMaterial);
	AGASPlayerController->SetShowMouseCursorAndForceRefresh(false);
}

void AAGASCharacter::HideMagicCircleOnPlayerController_Implementation()
{
	if (!IsLocallyControlled()) return;
	
	AAGASPlayerController* AGASPlayerController = GetController<AAGASPlayerController>();
	check(AGASPlayerController);
	
	AGASPlayerController->HideMagicCircle();
	AGASPlayerController->SetShowMouseCursorAndForceRefresh(true);
}

// function where we can save the location of the character so that we can potentially refactor "last saved position" to be
// an actual position rather than just a checkpoint (part of a TODO)
void AAGASCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AAGASGameModeBase* AGASGameMode = Cast<AAGASGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AGASGameMode)
	{
		UAGASLoadMenuSaveGame* SaveData = AGASGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;
		
		SaveData->PlayerStartTag = CheckpointTag;
		
		if (AAGASPlayerState* AGASPlayerState = Cast<AAGASPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = AGASPlayerState->GetPlayerLevel();
			SaveData->XPPoints = AGASPlayerState->GetXPPoints();
			SaveData->SpellPoints = AGASPlayerState->GetSpellPoints();
			SaveData->AttributePoints = AGASPlayerState->GetAttributePoints();
		}
		SaveData->Strength = UAGASAttributeSet::GetStrengthAttribute().GetGameplayAttributeData(AttributeSet)->GetBaseValue();
		SaveData->Intelligence = UAGASAttributeSet::GetIntelligenceAttribute().GetGameplayAttributeData(AttributeSet)->GetBaseValue();
		SaveData->Resilience = UAGASAttributeSet::GetResilienceAttribute().GetGameplayAttributeData(AttributeSet)->GetBaseValue();
		SaveData->Vigor = UAGASAttributeSet::GetVigorAttribute().GetGameplayAttributeData(AttributeSet)->GetBaseValue();
		
		AGASGameMode->SaveInGameProgressData(SaveData);
	}
}

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


