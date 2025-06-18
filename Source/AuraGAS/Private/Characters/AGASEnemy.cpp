// Copyright Dustin Carmelo


#include "Characters/AGASEnemy.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "AuraGAS/AuraGAS.h"
#include "Components/WidgetComponent.h"
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


void AAGASEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitializeAbilityActorInfo();

	InitializeOverheadHealthBar();
}

void AAGASEnemy::InitializeAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AbilityActorInfoSet();
	// TODO: temporary stats placeholder for testing
	InitializeDefaultStats();
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
