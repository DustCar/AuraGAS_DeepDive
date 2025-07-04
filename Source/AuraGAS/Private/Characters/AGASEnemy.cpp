// Copyright Dustin Carmelo


#include "Characters/AGASEnemy.h"

#include "AGASGameplayTags.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAbilitySystemLibrary.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "AuraGAS/AuraGAS.h"
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

	AttributeSet = CreateDefaultSubobject<UAGASAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
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

void AAGASEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitializeAbilityActorInfo();
	UAGASAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);

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

	UAGASAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAGASEnemy::InitializeOverheadHealthBar()
{
	checkf(OverheadHPBarWidgetControllerClass, TEXT("WidgetControllerClass not set in BP_EnemyBase."))

	if (OverheadHPBarWidgetController == nullptr)
	{
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
}
