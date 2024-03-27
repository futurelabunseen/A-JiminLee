
#pragma once

#include "UNGameplayTag.h"

#define UNTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define UNTAG_CHARACTER_STATE_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define UNTAG_CHARACTER_STATE_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))

#define UNTAG_GameplayCue_CHARACTER_AttackHit FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))
#define UNTAG_EVENT_CHARACTER_WEAPONEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Equip"))
#define UNTAG_EVENT_CHARACTER_WEAPONUNEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.UnEquip"))