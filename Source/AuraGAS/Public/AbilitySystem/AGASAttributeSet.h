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

	FGameplayEffectContextHandle EffectContextHandle;

	TSharedPtr<FEffectProperties> SourceProperties;

	TSharedPtr<FEffectProperties> TargetProperties;
};
/**
 * 
 */
UCLASS()
class AURAGAS_API  UAGASAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAGASAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	void MaximizeVitalAttributes();

	/**
	 * Vital Attributes Accessors
	 */
	ATTRIBUTE_ACCESSORS(ThisClass, HealthPoints)
	ATTRIBUTE_ACCESSORS(ThisClass, ManaPoints)

	/**
	 * Primary Attributes Accessors
	 */
	ATTRIBUTE_ACCESSORS(ThisClass, Strength)
	ATTRIBUTE_ACCESSORS(ThisClass, Intelligence)
	ATTRIBUTE_ACCESSORS(ThisClass, Resilience)
	ATTRIBUTE_ACCESSORS(ThisClass, Vigor)

	/**
	 * Secondary Attributes Accessors
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

	/**
	 * Resistance Attributes Accessors
	 */
	ATTRIBUTE_ACCESSORS(ThisClass, FireResistance)
	ATTRIBUTE_ACCESSORS(ThisClass, LightningResistance)
	ATTRIBUTE_ACCESSORS(ThisClass, ArcaneResistance)
	ATTRIBUTE_ACCESSORS(ThisClass, PhysicalResistance)

	/**
	 * Meta Attributes Accessors
	 */
	ATTRIBUTE_ACCESSORS(ThisClass, IncomingDamage)
	ATTRIBUTE_ACCESSORS(ThisClass, IncomingXPPoints)

	/**
	 * Vital Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthPoints, Category = "VitalAttributes")
	FGameplayAttributeData HealthPoints;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaPoints, Category = "VitalAttributes")
	FGameplayAttributeData ManaPoints;

	/**
	 * Primary Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "PrimaryAttributes")
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "PrimaryAttributes")
	FGameplayAttributeData Intelligence;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "PrimaryAttributes")
	FGameplayAttributeData Resilience;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "PrimaryAttributes")
	FGameplayAttributeData Vigor;

	/**
	 * Secondary Attributes 
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "SecondaryAttributes")
	FGameplayAttributeData Armor;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "SecondaryAttributes")
	FGameplayAttributeData ArmorPenetration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "SecondaryAttributes")
	FGameplayAttributeData BlockChance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "SecondaryAttributes")
	FGameplayAttributeData CriticalHitChance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "SecondaryAttributes")
	FGameplayAttributeData CriticalHitDamage;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "SecondaryAttributes")
	FGameplayAttributeData CriticalHitResistance;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "SecondaryAttributes")
	FGameplayAttributeData HealthRegeneration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "SecondaryAttributes")
	FGameplayAttributeData ManaRegeneration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealthPoints, Category = "SecondaryAttributes")
	FGameplayAttributeData MaxHealthPoints;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxManaPoints, Category = "SecondaryAttributes")
	FGameplayAttributeData MaxManaPoints;

	/**
	 * Resistance Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "SecondaryAttributes")
	FGameplayAttributeData FireResistance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "SecondaryAttributes")
	FGameplayAttributeData LightningResistance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "SecondaryAttributes")
	FGameplayAttributeData ArcaneResistance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "SecondaryAttributes")
	FGameplayAttributeData PhysicalResistance;

	/**
	 *	Meta Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category = "MetaAttributes")
	FGameplayAttributeData IncomingDamage;

	UPROPERTY(BlueprintReadOnly, Category = "MetaAttributes")
	FGameplayAttributeData IncomingXPPoints;
	
protected:
	
	//~ Begin Vital attribute rep notifies
	UFUNCTION()
	void OnRep_HealthPoints(const FGameplayAttributeData& OldHealthPoints) const;

	UFUNCTION()
	void OnRep_ManaPoints(const FGameplayAttributeData& OldManaPoints) const;
	//~ End Vital attribute rep notifies
	
	//~ Begin Primary Attributes rep notifies
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	//~ End Primary Attributes rep notifies
	
	//~ Begin Secondary Attributes rep notifies
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
	//~ End Secondary Attributes rep notifies

	//~ Begin Resistance Attributes rep notifies
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;
	//~ End Resistance Attributes rep notifies

private:
	void HandleIncomingDamage(const FEffectPropertiesAdvanced& Props);
	void Debuff(const FEffectPropertiesAdvanced& Props);
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectPropertiesAdvanced& Props);
	void ShowFloatingText(const FEffectPropertiesAdvanced& Props, float Damage, const bool bCriticalHit, const bool bBlockedHit) const;
	void SendXPPointsEvent(const FEffectPropertiesAdvanced& Props);
};


