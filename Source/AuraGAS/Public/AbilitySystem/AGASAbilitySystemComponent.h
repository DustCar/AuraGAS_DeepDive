// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AGASAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*Asset Tags*/)
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven)
// a delegate that will be used to loop through the players activatable abilities
// placed in ASC to allow for safe iteration using AbilityListLock
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&)

/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// function to call after Ability Actor info is set in AbilitySystemComponent, useful for binding delegates
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenSignature;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	// callback for ForEachAbility delegate
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
protected:

	virtual void OnRep_ActivateAbilities() override;
	
	// Callback function that is called when a GE is applied (delegate: OnGameplayEffectAppliedDelegateToSelf)
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
