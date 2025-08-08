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
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

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
	WeaponMesh->SetRenderCustomDepth(true);
}

void AAGASEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);
}

int32 AAGASEnemy::GetPlayerLevel()
{
	return Level;
}


void AAGASEnemy::Die()
{
	SetLifeSpan(CorpseLifeSpan);
	Super::Die();
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
}

void AAGASEnemy::InitializeAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AbilityActorInfoSet();

	if (HasAuthority())
	{
		UAGASAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
	}
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
		if (HasAuthority()) OverheadHPBarWidgetController->BroadcastInitialValues();
	}
}

void AAGASEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	if (!HasAuthority()) return;

	AGASAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}
