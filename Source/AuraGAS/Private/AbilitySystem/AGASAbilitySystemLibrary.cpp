// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAbilitySystemLibrary.h"

#include "AGASAbilityTypes.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "GameMode/AGASGameModeBase.h"
#include "Kismet/GameplayStatics.h"
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

void UAGASAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level, UAGASAbilitySystemComponent* ASC)
{
	UAGASCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	AActor* AvatarActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, EffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);

	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, EffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, EffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
	
}

void UAGASAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAGASAbilitySystemComponent* ASC)
{
	UAGASCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass);
		ASC->GiveAbility(AbilitySpec);
	}
}

UAGASCharacterClassInfo* UAGASAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAGASGameModeBase* AGASGameMode = Cast<AAGASGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AGASGameMode == nullptr) return nullptr;

	return AGASGameMode->CharacterClassInfo;
}

bool UAGASAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAGASGameplayEffectContext* AGASEffectContext = static_cast<const FAGASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AGASEffectContext->IsBlockedHit();
	}

	return false;
}

bool UAGASAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAGASGameplayEffectContext* AGASEffectContext = static_cast<const FAGASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AGASEffectContext->IsCriticalHit();
	}

	return false;
}

void UAGASAbilitySystemLibrary::SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FAGASGameplayEffectContext* AGASEffectContext = static_cast<FAGASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AGASEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAGASAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FAGASGameplayEffectContext* AGASEffectContext = static_cast<FAGASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AGASEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}
