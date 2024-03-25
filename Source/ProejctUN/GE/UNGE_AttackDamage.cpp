// Fill out your copyright notice in the Description page of Project Settings.


#include "../GE/UNGE_AttackDamage.h"
#include "../Attribute/UNCharacterAttributeSet.h"

UUNGE_AttackDamage::UUNGE_AttackDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UUNCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UUNCharacterAttributeSet, Health)));
	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	FScalableFloat DamageAmout(-30.f);
	FGameplayEffectModifierMagnitude ModMagnitude(DamageAmout);

	HealthModifier.ModifierMagnitude = ModMagnitude;
	Modifiers.Add(HealthModifier);
}
