// Copyright Dustin Carmelo


#include "UI/HUD/AGASLoadMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_AGASLoadMenu.h"
#include "UI/Widget/AGASLoadMenuWidget.h"

void AAGASLoadMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	LoadMenuViewModel = NewObject<UMVVM_AGASLoadMenu>(this, LoadMenuViewModelClass);
	LoadMenuViewModel->InitializeLoadSlots();
	
	LoadMenuWidget = CreateWidget<UAGASLoadMenuWidget>(GetWorld(), LoadMenuWidgetClass);
	LoadMenuWidget->AddToViewport();
	LoadMenuWidget->BlueprintInitializeWidget();
	
	LoadMenuViewModel->LoadData();
}
