// Copyright Dustin Carmelo


#include "AbilitySystem/AbilityTasks/AbilityTask_TargetDataUnderMouse.h"

#include "Player/AGASPlayerController.h"

UAbilityTask_TargetDataUnderMouse* UAbilityTask_TargetDataUnderMouse::CreateTargetDataUnderMouse(
	UGameplayAbility* OwningAbility)
{
	UAbilityTask_TargetDataUnderMouse* MyObj = NewAbilityTask<UAbilityTask_TargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UAbilityTask_TargetDataUnderMouse::Activate()
{
	AAGASPlayerController* PC = Cast<AAGASPlayerController>(Ability->GetCurrentActorInfo()->PlayerController);
	if (PC == nullptr) return;

	ValidData.Broadcast(PC->CursorHit.ImpactPoint);
}
