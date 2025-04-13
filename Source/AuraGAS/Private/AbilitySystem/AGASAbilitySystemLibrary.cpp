// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAbilitySystemLibrary.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "Player/AGASPlayerState.h"
#include "UI/HUD/AGASHUD.h"
#include "UI/WidgetController/AGASWidgetController.h"

UAGASOverlayWidgetController* UAGASAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AAGASHUD* HUD = Cast<AAGASHUD>(PC->GetHUD()))
		{
			AAGASPlayerState* PS = PC->GetPlayerState<AAGASPlayerState>();
			UAGASAbilitySystemComponent* ASC = Cast<UAGASAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			UAGASAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return HUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAGASAttributeMenuWidgetController* UAGASAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AAGASHUD* HUD = Cast<AAGASHUD>(PC->GetHUD()))
		{
			AAGASPlayerState* PS = PC->GetPlayerState<AAGASPlayerState>();
			UAGASAbilitySystemComponent* ASC = Cast<UAGASAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			UAGASAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return HUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}
