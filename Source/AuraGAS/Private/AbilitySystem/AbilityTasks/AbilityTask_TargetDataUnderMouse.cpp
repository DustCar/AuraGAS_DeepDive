// Copyright Dustin Carmelo


#include "AbilitySystem/AbilityTasks/AbilityTask_TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "Player/AGASPlayerController.h"

UAbilityTask_TargetDataUnderMouse* UAbilityTask_TargetDataUnderMouse::CreateTargetDataUnderMouse(
	UGameplayAbility* OwningAbility)
{
	UAbilityTask_TargetDataUnderMouse* MyObj = NewAbilityTask<UAbilityTask_TargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UAbilityTask_TargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		// Client sends TargetData including the HitResult to server
		SendTargetDataToServer();
	}
	else
	{
		// Server receives TargetData to broadcast on the server
		ReceiveTargetDataFromClient();
	}
	
}

void UAbilityTask_TargetDataUnderMouse::SendTargetDataToServer()
{
	// Starts the prediction window to encompass everything after the struct construction, to be used with AbilitySystemComponent->ScopedPredictionKey
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	AAGASPlayerController* PC = Cast<AAGASPlayerController>(Ability->GetCurrentActorInfo()->PlayerController);
	if (PC == nullptr) return;

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = PC->GetCursorHit();
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UAbilityTask_TargetDataUnderMouse::ReceiveTargetDataFromClient()
{
	const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
	const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
	AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);
	// re-broadcast TargetData to execute callback IF TargetData has already been sent before the callback was binded
	const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
	// else wait for the TargetData to be sent before callback is called
	if (!bCalledDelegate)
	{
		SetWaitingOnRemotePlayerData();
	}
}

void UAbilityTask_TargetDataUnderMouse::OnTargetDataReplicatedCallback(
	const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
