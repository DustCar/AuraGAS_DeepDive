// Copyright Dustin Carmelo


#include "Characters/AGASEnemy.h"

#include "AGASGameplayTags.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "AI/AGASAIController.h"
#include "AuraGAS/AuraGAS.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AGASUserWidget.h"
#include "UI/WidgetController/AGASProgressBarWidgetController.h"


AAGASEnemy::AAGASEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	if (IsValid(WeaponMesh))
	{
		WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}

	AbilitySystemComponent = CreateDefaultSubobject<UAGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UAGASAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	// Added so that any spawned enemy automatically gets an AI Controller on spawn
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CharacterClass = ECharacterClass::Elementalist;
	
	BaseWalkSpeed = 250.f;
}

void AAGASEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AGASAIController = Cast<AAGASAIController>(NewController);
	AGASAIController->GetBlackboardComponent()->InitializeBlackboard(*EnemyBehaviorTree->BlackboardAsset);
	AGASAIController->RunBehaviorTree(EnemyBehaviorTree);
	AGASAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AGASAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AAGASEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	if (IsValid(WeaponMesh))
	{
		WeaponMesh->SetRenderCustomDepth(true);
	}
}

void AAGASEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	if (IsValid(WeaponMesh))
	{
		WeaponMesh->SetRenderCustomDepth(false);
	}
}

int32 AAGASEnemy::GetCharacterLevel_Implementation()
{
	return Level;
}

void AAGASEnemy::SetCharacterLevel_Implementation(int32 NewLevel)
{
	Level = NewLevel;
}


void AAGASEnemy::Die(const FVector& DeathImpulse)
{
	AGASAIController->GetBrainComponent()->StopLogic(TEXT("Died"));
	SetLifeSpan(CorpseLifeSpan);
	Super::Die(DeathImpulse);
}

void AAGASEnemy::KnockbackCharacter_Implementation(const FVector& KnockbackForce)
{
	AGASAIController->StopMovement();
	Super::KnockbackCharacter_Implementation(KnockbackForce);
}

void AAGASEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAGASEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAGASEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	Tags.Emplace(ACTOR_TAG_ENEMY);
	InitializeAbilityActorInfo();

	if (HasAuthority())
	{
		UAGASAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	InitializeOverheadHealthBar();

	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&ThisClass::HitReactTagChanged
	);
	
	OnBeingShocked.BindLambda([this](bool bInIsBeingShocked)
	{
		GetCharacterMovement()->MaxWalkSpeed = bInIsBeingShocked ? 0.f : BaseWalkSpeed;
	});
}

void AAGASEnemy::InitializeAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AbilityActorInfoSet();
	
	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);

	if (HasAuthority())
	{
		UAGASAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
	}
	OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void AAGASEnemy::InitializeOverheadHealthBar()
{
	checkf(OverheadHPBarWidgetControllerClass, TEXT("WidgetControllerClass not set in BP_EnemyBase."))

	if (OverheadHPBarWidgetController == nullptr)
	{
		// Create a new progress bar widget controller and set it
		OverheadHPBarWidgetController = NewObject<UAGASProgressBarWidgetController>(this, OverheadHPBarWidgetControllerClass);
		FWidgetControllerParams WCParams(nullptr, nullptr, AbilitySystemComponent, AttributeSet);
		OverheadHPBarWidgetController->SetWidgetControllerParams(WCParams);
		OverheadHPBarWidgetController->BindCallbacksToDependencies();
	}

	if (UAGASUserWidget* AGASHealthBarWidget = Cast<UAGASUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AGASHealthBarWidget->SetWidgetController(OverheadHPBarWidgetController);
		OverheadHPBarWidgetController->BroadcastInitialValues();
	}
}

void AAGASEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	if (!HasAuthority()) return;

	AGASAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

void AAGASEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	
	if (!HasAuthority()) return;

	AGASAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
}
