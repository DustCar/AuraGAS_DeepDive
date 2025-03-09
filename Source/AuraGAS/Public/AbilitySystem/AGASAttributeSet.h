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
	ATTRIBUTE_ACCESSORS(ThisClass, ManaPoints)

	/**
	 * Main Stats Accessors
	 */
	ATTRIBUTE_ACCESSORS(ThisClass, Strength)
	ATTRIBUTE_ACCESSORS(ThisClass, Intelligence)
	ATTRIBUTE_ACCESSORS(ThisClass, Resilience)
	ATTRIBUTE_ACCESSORS(ThisClass, Vigor)

	/**
	 * Secondary Stats Accessors
	 */
	ATTRIBUTE_ACCESSORS(ThisClass, Armor)
	ATTRIBUTE_ACCESSORS(ThisClass, ArmorPenetration)
	ATTRIBUTE_ACCESSORS(ThisClass, BlockChance)
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalHitChance)
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalHitDamage)
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalHitResistance)
	ATTRIBUTE_ACCESSORS(ThisClass, HealthRegeneration)
	ATTRIBUTE_ACCESSORS(ThisClass, ManaRegeneration)
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealthPoints)
	ATTRIBUTE_ACCESSORS(ThisClass, MaxManaPoints)
	
protected:
	/**
	 * Vital Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthPoints, Category = "VitalAttributes")
	FGameplayAttributeData HealthPoints;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaPoints, Category = "VitalAttributes")
	FGameplayAttributeData ManaPoints;
	
	//~ Begin Vital attribute rep notifies
	UFUNCTION()
	void OnRep_HealthPoints(const FGameplayAttributeData& OldHealthPoints) const;

	UFUNCTION()
	void OnRep_ManaPoints(const FGameplayAttributeData& OldManaPoints) const;
	//~ End Vital attribute rep notifies

	/**
	 * Main Stats
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "MainStats")
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "MainStats")
	FGameplayAttributeData Intelligence;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "MainStats")
	FGameplayAttributeData Resilience;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "MainStats")
	FGameplayAttributeData Vigor;

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

	/**
	 * Secondary Stats
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "SecondaryStats")
	FGameplayAttributeData Armor;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "SecondaryStats")
	FGameplayAttributeData ArmorPenetration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "SecondaryStats")
	FGameplayAttributeData BlockChance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "SecondaryStats")
	FGameplayAttributeData CriticalHitChance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "SecondaryStats")
	FGameplayAttributeData CriticalHitDamage;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "SecondaryStats")
	FGameplayAttributeData CriticalHitResistance;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "SecondaryStats")
	FGameplayAttributeData HealthRegeneration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "SecondaryStats")
	FGameplayAttributeData ManaRegeneration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealthPoints, Category = "SecondaryStats")
	FGameplayAttributeData MaxHealthPoints;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxManaPoints, Category = "SecondaryStats")
	FGameplayAttributeData MaxManaPoints;
	
	//~ Begin Secondary Stats rep notifies
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
    
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
    
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
    
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
    
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
    
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
    
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
    
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
    void OnRep_MaxHealthPoints(const FGameplayAttributeData& OldMaxHealthPoints) const;
    
    UFUNCTION()
    void OnRep_MaxManaPoints(const FGameplayAttributeData& OldMaxManaPoints) const;
	//~ End Secondary Stats rep notifies

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectPropertiesAdvanced& Props);
};


