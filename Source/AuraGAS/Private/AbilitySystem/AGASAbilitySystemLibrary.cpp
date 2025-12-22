// Copyright Dustin Carmelo


#include "AbilitySystem/AGASAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AGASAbilityTypes.h"
#include "AGASGameplayTags.h"
#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AuraGAS/AuraGAS.h"
#include "Engine/OverlapResult.h"
#include "Game/AGASGameInstance.h"
#include "Interaction/AGASCombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AGASPlayerController.h"
#include "Player/AGASPlayerState.h"
#include "UI/HUD/AGASHUD.h"
#include "UI/WidgetController/AGASWidgetController.h"

bool UAGASAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams)
{
	if (AAGASPlayerController* PC = CastChecked<AAGASPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController()))
	{
		AAGASPlayerState* PS = PC->GetPlayerState<AAGASPlayerState>();
		UAGASAbilitySystemComponent* ASC = PS->GetAGASAbilitySystemComponent();
		UAGASAttributeSet* AS = PS->GetAttributeSet();

		OutWCParams.PlayerState = PS;
		OutWCParams.PlayerController = PC;
		OutWCParams.AbilitySystemComponent = ASC;
		OutWCParams.AttributeSet = AS;
		return true;
	}
	
	return false;
}

UAGASOverlayWidgetController* UAGASAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams))
	{
		if (AAGASHUD* HUD = Cast<AAGASHUD>(WCParams.PlayerController->GetHUD()))
		{
			return HUD->GetOverlayWidgetController(WCParams);
		}
	}

	return nullptr;
}

UAGASAttributeMenuWidgetController* UAGASAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams))
	{
		if (AAGASHUD* HUD = Cast<AAGASHUD>(WCParams.PlayerController->GetHUD()))
		{
			return HUD->GetAttributeMenuWidgetController(WCParams);
		}
	}

	return nullptr;
}

UAGASSpellMenuWidgetController* UAGASAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams))
	{
		if (AAGASHUD* HUD = Cast<AAGASHUD>(WCParams.PlayerController->GetHUD()))
		{
			return HUD->GetSpellMenuWidgetController(WCParams);
		}
	}

	return nullptr;
}

void UAGASAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAGASAbilitySystemComponent* ASC)
{
	UAGASCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	AActor* AvatarActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);

	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
	
}

void UAGASAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAGASAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UAGASCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	
	for (TSubclassOf<UGameplayAbility> CommonAbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbilityClass);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> UniqueAbilityClass : DefaultInfo.UniqueAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UAGASCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(UniqueAbilityClass, IAGASCombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

int32 UAGASAbilitySystemLibrary::GetXPPointsRewarded(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const int32 CharacterLevel)
{
	UAGASCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	return ClassDefaultInfo.XPPointsRewarded.AsInteger(CharacterLevel);
}

UAGASCharacterClassInfo* UAGASAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	UAGASGameInstance* AGASGameInstance = Cast<UAGASGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (AGASGameInstance == nullptr) return nullptr;

	return AGASGameInstance->CharacterClassInfo;
}

UAGASAbilityInfo* UAGASAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	UAGASGameInstance* AGASGameInstance = Cast<UAGASGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (AGASGameInstance == nullptr) return nullptr;

	return AGASGameInstance->AbilityInfo;
}

FGameplayEffectContextHandle UAGASAbilitySystemLibrary::ApplyDamageEffectToTarget(const FDamageEffectParams& InParams)
{
	UAbilitySystemComponent* SourceASC = InParams.SourceAbilitySystemComponent;
	UAbilitySystemComponent* TargetASC = InParams.TargetAbilitySystemComponent;
	
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceASC->GetAvatarActor());
	
	 const FGameplayEffectSpecHandle DamageEffectSpecHandle = SourceASC->MakeOutgoingSpec(InParams.DamageGameplayEffectClass, InParams.AbilityLevel, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, InParams.DamageType, InParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, TAG_Debuff_Params_Chance, InParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, TAG_Debuff_Params_Damage, InParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, TAG_Debuff_Params_Frequency, InParams.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, TAG_Debuff_Params_Duration, InParams.DebuffDuration);
	
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, TargetASC);
	
	return EffectContextHandle;
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

void UAGASAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FAGASGameplayEffectContext* AGASEffectContext = static_cast<FAGASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AGASEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAGASAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
	const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);

		for (FOverlapResult& Overlap : Overlaps)
		{
			if (!Overlap.GetActor()->Implements<UAGASCombatInterface>() || IAGASCombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				continue;
			}

			OutOverlappingActors.AddUnique(Overlap.GetActor());
		}
	}
}

bool UAGASAbilitySystemLibrary::IsOnSameTeam(const AActor* FirstActor, const AActor* SecondActor)
{
	const bool bBothPlayers = FirstActor->ActorHasTag(ACTOR_TAG_PLAYER) && SecondActor->ActorHasTag(ACTOR_TAG_PLAYER);
	const bool bBothEnemies = FirstActor->ActorHasTag(ACTOR_TAG_ENEMY) && SecondActor->ActorHasTag(ACTOR_TAG_ENEMY);

	return bBothPlayers || bBothEnemies;
}
