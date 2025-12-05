// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AGASAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*Asset Tags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
// a delegate that will be used to loop through the players activatable abilities
// placed in ASC to allow for safe iteration using AbilityListLock
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*Ability Tag*/, const FGameplayTag& /*Status Tag*/, int32 /*Ability Level*/);

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
	// needed to let the widget controllers know to update the UI (used by: SpellMenuWC)
	FAbilityStatusChanged AbilityStatusChanged;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	// callback for ForEachAbility delegate
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	// obtain the gameplay ability spec from the player's activatable abilities array that matches the tag
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	
	// function to run whenever the player levels up to update any possible/eligible spells/abilities on the
	// player's spell menu
	void UpdateAbilityStatuses(int32 Level);
	
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
	
protected:

	virtual void OnRep_ActivateAbilities() override;
	
	// Callback function that is called when a GE is applied (delegate: OnGameplayEffectAppliedDelegateToSelf)
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
	// Client RPC to broadcast the new status tag for a given ability that is unlockable at a certain level
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};
