// Copyright Dustin Carmelo

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AGASAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Base struct for GameplayEffect properties
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<AActor> AvatarActor = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<AController> Controller = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> Character = nullptr;
};

// Advanced struct that holds the effect context and the source and target effect properties
USTRUCT()
struct FEffectPropertiesAdvanced
{
	GENERATED_BODY()

	FEffectPropertiesAdvanced()
	{
		SourceProperties = MakeShared<FEffectProperties>();
		TargetProperties = MakeShared<FEffectProperties>();
	}

	FGameplayEffectContextHandle EffectContext;

	TSharedPtr<FEffectProperties> SourceProperties;

	TSharedPtr<FEffectProperties> TargetProperties;
};
/**
 * 
 */
UCLASS()
class AURAGAS_API UAGASAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAGASAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthPoints, Category = "MainStats")
	FGameplayAttributeData HealthPoints;
	ATTRIBUTE_ACCESSORS(ThisClass, HealthPoints)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealthPoints, Category = "MainStats")
	FGameplayAttributeData MaxHealthPoints;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealthPoints)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaPoints, Category = "MainStats")
	FGameplayAttributeData ManaPoints;
	ATTRIBUTE_ACCESSORS(ThisClass, ManaPoints)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxManaPoints, Category = "MainStats")
	FGameplayAttributeData MaxManaPoints;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxManaPoints)
	
	//~ Begin Main attribute rep notifies
	UFUNCTION()
	void OnRep_HealthPoints(const FGameplayAttributeData& OldHealthPoints) const;

	UFUNCTION()
	void OnRep_MaxHealthPoints(const FGameplayAttributeData& OldMaxHealthPoints) const;

	UFUNCTION()
	void OnRep_ManaPoints(const FGameplayAttributeData& OldManaPoints) const;

	UFUNCTION()
	void OnRep_MaxManaPoints(const FGameplayAttributeData& OldMaxManaPoints) const;
	//~ End Main attribute rep notifies

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectPropertiesAdvanced& Props);
};





