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
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AGASAttributeSet->GetHealthPointsAttribute()).AddUObject(this, &ThisClass::HealthPointsChanged);

	// binds MaxHealthPointsChanged to max HP attribute
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AGASAttributeSet->GetMaxHealthPointsAttribute()).AddUObject(this, &ThisClass::MaxHealthPointsChanged);

	// binds ManaPointsChanged to MP attribute
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AGASAttributeSet->GetManaPointsAttribute()).AddUObject(this, &ThisClass::ManaPointsChanged);

	// binds MaxManaPointsChanged to max MP attribute
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AGASAttributeSet->GetMaxManaPointsAttribute()).AddUObject(this, &ThisClass::MaxManaPointsChanged);

	// use of lambda allows us to avoid declaring multiple callbacks for simpler code like this and the attribute changes
	// I will leave the attribute change callbacks up for now, but may change it to lambdas if functionality stays
	AbilitySystemComponent->EffectAssetTags.AddLambda(
		[] (const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
			}
		}
	);
}

// callback for when HP changes
void UAGASOverlayWidgetController::HealthPointsChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthPointsChanged.Broadcast(Data.NewValue);
}

// callback for when max HP changes
void UAGASOverlayWidgetController::MaxHealthPointsChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthPointsChanged.Broadcast(Data.NewValue);
}

// callback for when MP changes
void UAGASOverlayWidgetController::ManaPointsChanged(const FOnAttributeChangeData& Data) const
{
	OnManaPointsChanged.Broadcast(Data.NewValue);
}

// callback for when max MP changes
void UAGASOverlayWidgetController::MaxManaPointsChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaPointsChanged.Broadcast(Data.NewValue);
}
