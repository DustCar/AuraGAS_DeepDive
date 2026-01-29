// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_AGASLoadSlot.generated.h"

UENUM(BlueprintType)
enum ELoadSlotWidget
{
	Vacant,
	EnterName,
	Taken
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSaveButton, bool, bEnable);
/**
 * 
 */
UCLASS()
class AURAGAS_API UMVVM_AGASLoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSaveButton EnableSelectSaveButton;
	
	void SetLoadSlotWidget(const ELoadSlotWidget LoadSlotWidget);
	
	UPROPERTY()
	FString LoadSlotName;
	
	UPROPERTY()
	TEnumAsByte<ELoadSlotWidget> SlotStatus;
	
	/* Field Notifies */ 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString PlayerName;
	
	void SetPlayerName(FString InPlayerName);
	
	FString GetPlayerName() const { return PlayerName; }
};
