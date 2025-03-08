// Copyright Dustin Carmelo


#include "UI/WidgetController/AGASOverlayWidgetController.h"

#include "AbilitySystem/AGASAbilitySystemComponent.h"
#include "AbilitySystem/AGASAttributeSet.h"

// initially broadcast the starter values of attributes to delegates
void UAGASOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthPointsChanged.Broadcast(AGASAttributeSet->GetHealthPoints());
	OnMaxHealthPointsChanged.Broadcast(AGASAttributeSet->GetMaxHealthPoints());
	OnManaPointsChanged.Broadcast(AGASAttributeSet->GetManaPoints());
	OnMaxManaPointsChanged.Broadcast(AGASAttributeSet->GetMaxManaPoints());
}

// bind our own callbacks to the attribute change delegates on ASC
void UAGASOverlayWidgetController::BindCallbacksToDependencies()
{
	/**
	 * The GameplayAttributeValueChangeDelegate is a delegate that is part of the ASC
	 * and is triggered whenever the ASC sees some change to the attribute that is passed to the
	 * delegate.
	 */
	
	// binds HealthPointsChanged to HP attribute
	BindAttributeChange(AGASAttributeSet->GetHealthPointsAttribute(), OnHealthPointsChanged);

	// binds MaxHealthPointsChanged to max HP attribute
	BindAttributeChange(AGASAttributeSet->GetMaxHealthPointsAttribute(), OnMaxHealthPointsChanged);

	// binds ManaPointsChanged to MP attribute
	BindAttributeChange(AGASAttributeSet->GetManaPointsAttribute(), OnManaPointsChanged);

	// binds MaxManaPointsChanged to max MP attribute
	BindAttributeChange(AGASAttributeSet->GetMaxManaPointsAttribute(), OnMaxManaPointsChanged);

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
	FOnAttributeChangedSignature& AttributeDelegate) const
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attribute).AddLambda(
			[this, &AttributeDelegate] (const FOnAttributeChangeData& Data)
			{
				AttributeDelegate.Broadcast(Data.NewValue);
			}
		);
}
