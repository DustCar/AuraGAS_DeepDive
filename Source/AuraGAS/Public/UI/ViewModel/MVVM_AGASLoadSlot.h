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
	TEnumAsByte<ELoadSlotWidget> SlotStatus;
	
	UPROPERTY()
	FName PlayerStartTag;
	
	/* Field Notifies */ 
	void SetPlayerName(FString InPlayerName);
	void SetMapName(FString InMapName);
	void SetLoadSlotName(FString InLoadSlotName);
	void SetPlayerLevel(int32 InPlayerLevel);
	
	FString GetPlayerName() const { return PlayerName; }
	FString GetMapName() const { return MapName; }
	FString GetLoadSlotName() const { return LoadSlotName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString PlayerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 PlayerLevel;
};
