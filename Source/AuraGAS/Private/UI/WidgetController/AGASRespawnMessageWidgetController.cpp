// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASRespawnMessageWidgetController.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "Characters/AGASCharacter.h"

void UAGASRespawnMessageWidgetController::BindCallbacksToDependencies()
{
	AAGASCharacter* AGASCharacter = Cast<AAGASCharacter>(AGASAbilitySystemComponent->GetAvatarActor());
	if (AGASCharacter)
	{
		AGASCharacter->OnDeathTimerSent.AddLambda([this](float NewValue)
		{
			OnDeathTimerUpdated.Broadcast(NewValue);
		});
	}
}
