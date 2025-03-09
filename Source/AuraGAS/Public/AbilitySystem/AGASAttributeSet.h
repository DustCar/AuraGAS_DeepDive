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

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/**
	 * Vital Attributes Accessors
	 */
	ATTRIBUTE_ACCESSORS(ThisClass, HealthPoints)
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealthPoints)
	ATTRIBUTE_ACCESSORS(ThisClass, ManaPoints)
	ATTRIBUTE_ACCESSORS(ThisClass, MaxManaPoints)

	/**
	 * Main Stats Accessors
	 */
	ATTRIBUTE_ACCESSORS(ThisClass, Strength)
	ATTRIBUTE_ACCESSORS(ThisClass, Intelligence)
	ATTRIBUTE_ACCESSORS(ThisClass, Resilience)
	ATTRIBUTE_ACCESSORS(ThisClass, Vigor)

protected:
	//~ Begin Vital Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthPoints, Category = "VitalAttributes")
	FGameplayAttributeData HealthPoints;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealthPoints, Category = "VitalAttributes")
	FGameplayAttributeData MaxHealthPoints;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaPoints, Category = "VitalAttributes")
	FGameplayAttributeData ManaPoints;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxManaPoints, Category = "VitalAttributes")
	FGameplayAttributeData MaxManaPoints;
	//~ End Vital Attributes
	
	//~ Begin Vital attribute rep notifies
	UFUNCTION()
	void OnRep_HealthPoints(const FGameplayAttributeData& OldHealthPoints) const;

	UFUNCTION()
	void OnRep_MaxHealthPoints(const FGameplayAttributeData& OldMaxHealthPoints) const;

	UFUNCTION()
	void OnRep_ManaPoints(const FGameplayAttributeData& OldManaPoints) const;

	UFUNCTION()
	void OnRep_MaxManaPoints(const FGameplayAttributeData& OldMaxManaPoints) const;
	//~ End Vital attribute rep notifies

	
	//~ Begin Main Stats
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "MainStats")
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "MainStats")
	FGameplayAttributeData Intelligence;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "MainStats")
	FGameplayAttributeData Resilience;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "MainStats")
	FGameplayAttributeData Vigor;
	//~ End Main Stats

	//~ Begin Main stat rep notifies
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	//~ End Main stat rep notifies

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectPropertiesAdvanced& Props);
};


