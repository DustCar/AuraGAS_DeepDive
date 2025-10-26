// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASOverlayWidgetController.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "AbilitySystem/Data/AGASAbilityInfo.h"
#include "AbilitySystem/Data/AGASLevelUpInfo.h"
#include "Player/AGASPlayerState.h"

// initially broadcast the starter values of attributes to delegates
void UAGASOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthPointsChanged.Broadcast(AGASAttributeSet->GetHealthPoints());
	OnMaxHealthPointsChanged.Broadcast(AGASAttributeSet->GetMaxHealthPoints());
	OnManaPointsChanged.Broadcast(AGASAttributeSet->GetManaPoints());
	OnMaxManaPointsChanged.Broadcast(AGASAttributeSet->GetMaxManaPoints());

	// place the automatic call here since BroadcastInitialValues is blueprint callable, and we'll be calling this
	// within the overlay WBP in Event Construct since our Widget Controller Set seems to be getting called late
	if (AbilitySystemComponent->bStartupAbilitiesGiven)
	{
		OnInitializedStartupAbilities();
	}
}

// bind our own callbacks to the attribute change delegates on ASC
void UAGASOverlayWidgetController::BindCallbacksToDependencies()
{
	AAGASPlayerState* AGASPlayerState = CastChecked<AAGASPlayerState>(PlayerState);

	AGASPlayerState->OnXPPointsChangedSignature.AddUObject(this, &ThisClass::OnXPPointsChanged);

	/**
	 * The GameplayAttributeValueChangeDelegate is a delegate that is part of the ASC
	 * and is triggered whenever the ASC sees some change to the attribute that is passed to the
	 * delegate.
	 */
	// binds HealthPointsChanged broadcast to HP attribute change delegate via lambda
	BindAttributeChange(AGASAttributeSet->GetHealthPointsAttribute(), OnHealthPointsChanged);

	// binds MaxHealthPointsChanged broadcast to max HP attribute change delegate via lambda
	BindAttributeChange(AGASAttributeSet->GetMaxHealthPointsAttribute(), OnMaxHealthPointsChanged);

	// binds ManaPointsChanged broadcast to MP attribute change delegate via lambda
	BindAttributeChange(AGASAttributeSet->GetManaPointsAttribute(), OnManaPointsChanged);

	// binds MaxManaPointsChanged broadcast to max MP attribute change delegate via lambda
	BindAttributeChange(AGASAttributeSet->GetMaxManaPointsAttribute(), OnMaxManaPointsChanged);

	// check to see if abilities are already given, if not then bind the callback to delegate
	if (!AbilitySystemComponent->bStartupAbilitiesGiven)
	{
		AbilitySystemComponent->AbilitiesGivenSignature.AddUObject(this, &ThisClass::OnInitializedStartupAbilities);
	}

	// use of lambda allows us to avoid declaring multiple callbacks for simpler code like this and the attribute changes
	// I will leave the attribute change callbacks up for now, but may change it to lambdas if functionality stays
	AbilitySystemComponent->EffectAssetTags.AddLambda(
		[this] (const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);

}

void UAGASOverlayWidgetController::BindAttributeChange(const FGameplayAttribute& Attribute,
                                                       FOnAttributeChanged& AttributeDelegate) const
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attribute).AddLambda(
			[this, &AttributeDelegate] (const FOnAttributeChangeData& Data)
			{
				AttributeDelegate.Broadcast(Data.NewValue);
			}
		);
}

void UAGASOverlayWidgetController::OnInitializedStartupAbilities()
{
	// TODO: Get information about all given abilities, loop up their ability info, and broadcast it to widgets
	if (!AbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility AbilitiesDelegate;
	AbilitiesDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(UAGASAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = UAGASAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});

	AbilitySystemComponent->ForEachAbility(AbilitiesDelegate);
}

void UAGASOverlayWidgetController::OnXPPointsChanged(int32 NewXPPoints) const 
{
	const AAGASPlayerState* AGASPlayerState = CastChecked<AAGASPlayerState>(PlayerState);
	const UAGASLevelUpInfo* LevelUpInfo = AGASPlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo on the Player State. Check the Player State BP"))

	const int32 FoundLevel = LevelUpInfo->FindLevelForXP(NewXPPoints);
	// Num - 1 since 0 is counted in Num so Num - 1 would be the appropriate max level
	// (ex. 10 is max level, meaning 11 elements since 0 is counted. So Num - 1, 11 - 1 for 10.)
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num() - 1;

	if (FoundLevel > 0 && FoundLevel <= MaxLevel)
	{
		const int32 CurrentLevelUpRequirement = LevelUpInfo->LevelUpInformation[FoundLevel].LevelUpRequirement;
		// minimum amount of XPPoints required to be at the current level
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[FoundLevel - 1].LevelUpRequirement;

		// amount of XP points needed before next level
		const int32 DeltaLevelUpRequirement = CurrentLevelUpRequirement - PreviousLevelUpRequirement;
		// amount of XP points gained currently at level
		const int32 XPPointsForThisLevel = NewXPPoints - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPPointsForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);
		OnXPPointsPercentChanged.Broadcast(XPBarPercent);
	}
}
