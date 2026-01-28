// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AGASLoadMenuHUD.generated.h"

class UMVVM_AGASLoadMenu;
class UAGASLoadMenuWidget;
/**
 * 
 */
UCLASS()
class AURAGAS_API AAGASLoadMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadMenuWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAGASLoadMenuWidget> LoadMenuWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_AGASLoadMenu> LoadMenuViewModelClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_AGASLoadMenu> LoadMenuViewModel;
	
protected:
	virtual void BeginPlay() override;
};
