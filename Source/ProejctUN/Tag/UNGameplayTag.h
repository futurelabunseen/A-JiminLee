
#pragma once

#include "UNGameplayTag.h"

#define UNTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define UNTAG_CHARACTER_STATE_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define UNTAG_CHARACTER_STATE_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))
#define UNTAG_CHARACTER_STATE_ISSTUNING FGameplayTag::RequestGameplayTag(FName("Character.State.IsStuning"))
#define UNTAG_CHARACTER_STATE_ISATTACKING FGameplayTag::RequestGameplayTag(FName("Character.State.IsAttacking"))
#define UNTAG_CHARACTER_STATE_ISULTIMATING FGameplayTag::RequestGameplayTag(FName("Character.State.IsUltimating"))
#define UNTAG_CHARACTER_STATE_ISSKILLING FGameplayTag::RequestGameplayTag(FName("Character.State.IsSkilling"))

#define UNTAG_GameplayCue_CHARACTER_AttackHit FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))
#define UNTAG_GAMEPLAYCUE_CHARACTER_FLOORSKILLEFFECT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.FloorSkillEffect"))
#define UNTAG_GAMEPLAYCUE_CHARACTER_TELEPORTEFFECT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.TeleportEffect"))
#define UNTAG_GAMEPLAYCUE_CHARACTER_INVINSIBLEEFFECT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.InvinsibleEffect"))
#define UNTAG_GAMEPLAYCUE_CHARACTER_STUNEFFECT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.StunEffect"))
#define UNTAG_GAMEPLAYCUE_PROPS_ULTIMATEFLOORFFECT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Props.UltimateFloorEffect"))
#define UNTAG_EVENT_CHARACTER_WEAPONEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Equip"))
#define UNTAG_EVENT_CHARACTER_WEAPONUNEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.UnEquip"))
