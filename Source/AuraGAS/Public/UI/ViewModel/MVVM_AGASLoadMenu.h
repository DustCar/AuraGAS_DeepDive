// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include "MVVM_AGASLoadMenu.generated.h"

class UMVVM_AGASLoadSlot;
/**
 * 
 */
UCLASS()
class AURAGAS_API UMVVM_AGASLoadMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	
	void InitializeLoadSlots();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_AGASLoadSlot> LoadSlotViewModelClass;
	
	UFUNCTION(BlueprintCallable)
	UMVVM_AGASLoadSlot* GetLoadSlotViewModelByIndex(int32 InIndex) const;
	
	UFUNCTION(BlueprintCallable)
	void NewSaveButtonPressed(int32 InSlot, const FString EnteredName);
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 InSlot);
	
	UFUNCTION(BlueprintCallable)
	void LoadSaveButtonPressed(int32 InSlot);
	
private:
	
	UPROPERTY()
	TMap<int32, UMVVM_AGASLoadSlot*> LoadSlots;
	
	UPROPERTY()
	TObjectPtr<UMVVM_AGASLoadSlot> LoadSlot_0;
	
	UPROPERTY()
	TObjectPtr<UMVVM_AGASLoadSlot> LoadSlot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_AGASLoadSlot> LoadSlot_2;
};
