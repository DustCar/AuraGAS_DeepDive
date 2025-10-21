// Copyright Dustin Carmelo


#include "AbilitySystem/AsyncTasks/AsyncTask_WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UAsyncTask_WaitCooldownChange* UAsyncTask_WaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	if (!IsValid(InAbilitySystemComponent) || !InCooldownTag.IsValid()) return nullptr;
	
	UAsyncTask_WaitCooldownChange* WaitCooldownChange = NewObject<UAsyncTask_WaitCooldownChange>();
	WaitCooldownChange->ASC = InAbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	// binds our cooldown tag changed function to the addition/removal of a cooldown tag like what we did with HitReact in Enemy
	// used to listen for when cooldown ends
	InAbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange, &UAsyncTask_WaitCooldownChange::CooldownTagChanged);

	// respond to when a duration gameplay effect is applied
	// used to listen for when a cooldown starts; recommended way to listen for when a cooldown begins
	InAbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UAsyncTask_WaitCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UAsyncTask_WaitCooldownChange::EndTask()
{
	if (IsValid(ASC))
	{
		// Unbind all the callback functions we bound
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
	}
	
	SetReadyToDestroy();
}

void UAsyncTask_WaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		OnCooldownEnd.Broadcast(-1.f);
	}
	// Alternative code for cooldown start: Rather than making another callback function that would be bound to OnActiveGameplayEffectAdded
	// we can just use an else statement here (for when the tag would have a count > 0 meaning active) and broadcast to CDStart
	// This approach is simpler and accomplishes something similar but does not have access to GameplayEffectSpec
	// else
	// {
	// 	const FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
	// 	const TArray<float> ActiveTimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
	// 	const float TimeRemaining = FMath::Max(ActiveTimesRemaining);
	//
	// 	CooldownStart.Broadcast(TimeRemaining);
	// }
}

// Recommended way to listen for a cooldown start
void UAsyncTask_WaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// boolean that checks if the ability is replicated or not
	// GetAbilityInstance_NotReplicated() is valid on predicted GE and null on replicated versions from server
	// we use a ! so that if it returns null then the bool is true meaning that it is replicated
	const bool bIsReplicatedEffect = !SpecApplied.GetContext().GetAbilityInstance_NotReplicated();
	if (bIsReplicatedEffect) return;
	
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		const FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		const TArray<float> ActiveTimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		const float TimeRemaining = FMath::Max(ActiveTimesRemaining);
		
		OnCooldownStart.Broadcast(TimeRemaining);
	}
}
