// Copyright Dustin Carmelo


#include "AGASGameplayTags.h"

// Primary Attributes Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Primary_Strength, "Attributes.Primary.Strength")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Primary_Resilience, "Attributes.Primary.Resilience")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Primary_Vigor, "Attributes.Primary.Vigor")

// Secondary Attributes Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_Armor, "Attributes.Secondary.Armor")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_MaxHealthPoints, "Attributes.Secondary.MaxHealthPoints")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Secondary_MaxManaPoints, "Attributes.Secondary.MaxManaPoints")

// Meta Attribute Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Meta_IncomingXPPoints, "Attributes.Meta.IncomingXPPoints")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Meta_IncomingForce, "Attributes.Meta.IncomingForce")

// Input Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_LMB, "InputTag.LMB")
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_RMB, "InputTag.RMB")
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_1, "InputTag.1")
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_2, "InputTag.2")
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_3, "InputTag.3")
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_4, "InputTag.4")
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Passive_1, "InputTag.Passive.1")
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Passive_2, "InputTag.Passive.2")

// Damage Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage, "Damage")
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Fire, "Damage.Fire")
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Lightning, "Damage.Lightning")
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Arcane, "Damage.Arcane")
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Physical, "Damage.Physical")

// Debuff Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Debuff, "Debuff")
UE_DEFINE_GAMEPLAY_TAG(TAG_Debuff_Burn, "Debuff.Burn")
UE_DEFINE_GAMEPLAY_TAG(TAG_Debuff_Stun, "Debuff.Stun")
UE_DEFINE_GAMEPLAY_TAG(TAG_Debuff_Arcane, "Debuff.Arcane")
UE_DEFINE_GAMEPLAY_TAG(TAG_Debuff_Physical, "Debuff.Physical")

// Debuff Tags: Params
UE_DEFINE_GAMEPLAY_TAG(TAG_Debuff_Params_Chance, "Debuff.Params.Chance")
UE_DEFINE_GAMEPLAY_TAG(TAG_Debuff_Params_Damage, "Debuff.Params.Damage")
UE_DEFINE_GAMEPLAY_TAG(TAG_Debuff_Params_Frequency, "Debuff.Params.Frequency")
UE_DEFINE_GAMEPLAY_TAG(TAG_Debuff_Params_Duration, "Debuff.Params.Duration")

// Resistance Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Resistance, "Attributes.Resistance")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Resistance_Fire, "Attributes.Resistance.Fire")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Resistance_Lightning, "Attributes.Resistance.Lightning")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Resistance_Arcane, "Attributes.Resistance.Arcane")
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Resistance_Physical, "Attributes.Resistance.Physical")

// Ability Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities, "Abilities")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Attack, "Abilities.Attack")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Summon, "Abilities.Summon")

UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_HitReact, "Abilities.HitReact")

// Ability Tags: Status
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Status_Locked, "Abilities.Status.Locked")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Status_Eligible, "Abilities.Status.Eligible")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Status_Unlocked, "Abilities.Status.Unlocked")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Status_Equipped, "Abilities.Status.Equipped")

// Ability Tags: Types
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Type_Offensive, "Abilities.Type.Offensive")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Type_Passive, "Abilities.Type.Passive")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Type_None, "Abilities.Type.None")

// Ability Tags: Aura
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Fire_FireBolt, "Abilities.Fire.FireBolt")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Fire_FireBlast, "Abilities.Fire.FireBlast")

UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Lightning_Electrocute, "Abilities.Lightning.Electrocute")

UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Arcane_ArcaneShards, "Abilities.Arcane.ArcaneShards")

UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Passive, "Abilities.Passive")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Passive_HaloOfProtection, "Abilities.Passive.HaloOfProtection")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Passive_LifeSiphon, "Abilities.Passive.LifeSiphon")
UE_DEFINE_GAMEPLAY_TAG(TAG_Abilities_Passive_ManaSiphon, "Abilities.Passive.ManaSiphon")

// Cooldown Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Cooldown_Fire_FireBolt, "Cooldown.Fire.FireBolt")
UE_DEFINE_GAMEPLAY_TAG(TAG_Cooldown_Fire_FireBlast, "Cooldown.Fire.FireBlast")

UE_DEFINE_GAMEPLAY_TAG(TAG_Cooldown_Lightning_Electrocute, "Cooldown.Lightning.Electrocute")

UE_DEFINE_GAMEPLAY_TAG(TAG_Cooldown_Arcane_ArcaneShards, "Cooldown.Arcane.ArcaneShards")

// Combat Socket Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_CombatSocket_Weapon, "CombatSocket.Weapon")
UE_DEFINE_GAMEPLAY_TAG(TAG_CombatSocket_RightHand, "CombatSocket.RightHand")
UE_DEFINE_GAMEPLAY_TAG(TAG_CombatSocket_LeftHand, "CombatSocket.LeftHand")
UE_DEFINE_GAMEPLAY_TAG(TAG_CombatSocket_Tail, "CombatSocket.Tail")

// Montage Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Montage_Attack_1, "Montage.Attack.1")
UE_DEFINE_GAMEPLAY_TAG(TAG_Montage_Attack_2, "Montage.Attack.2")
UE_DEFINE_GAMEPLAY_TAG(TAG_Montage_Attack_3, "Montage.Attack.3")
UE_DEFINE_GAMEPLAY_TAG(TAG_Montage_Attack_4, "Montage.Attack.4")

// Player Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Player_Block_InputPressed, "Player.Block.InputPressed")
UE_DEFINE_GAMEPLAY_TAG(TAG_Player_Block_InputHeld, "Player.Block.InputHeld")
UE_DEFINE_GAMEPLAY_TAG(TAG_Player_Block_InputReleased, "Player.Block.InputReleased")
UE_DEFINE_GAMEPLAY_TAG(TAG_Player_Block_CursorTrace, "Player.Block.CursorTrace")

// Other Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Effects_HitReact, "Effects.HitReact")
UE_DEFINE_GAMEPLAY_TAG(TAG_StatusEffects_Knockback, "StatusEffects.Knockback")
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_FireBlast, "GameplayCue.FireBlast")

const TMap<FGameplayTag, FGameplayTag> AGASGameplayTags::DamageTypeToResistMap = 
{
	{TAG_Damage_Fire, TAG_Attributes_Resistance_Fire},
	{TAG_Damage_Lightning, TAG_Attributes_Resistance_Lightning},
	{TAG_Damage_Arcane, TAG_Attributes_Resistance_Arcane},
	{TAG_Damage_Physical, TAG_Attributes_Resistance_Physical}
};

const TMap<FGameplayTag, FGameplayTag> AGASGameplayTags::DamageTypeToDebuffMap = 
{
	{TAG_Damage_Fire, TAG_Debuff_Burn},
	{TAG_Damage_Lightning, TAG_Debuff_Stun},
	{TAG_Damage_Arcane, TAG_Debuff_Arcane},
	{TAG_Damage_Physical, TAG_Debuff_Physical}
};

AGASGameplayTags::AGASGameplayTags()
{
}

AGASGameplayTags::~AGASGameplayTags()
{
}
