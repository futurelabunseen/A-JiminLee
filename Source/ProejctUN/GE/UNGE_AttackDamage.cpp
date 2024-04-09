// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/UNGE_AttackDamage.h"
#include "Attribute/UNCharacterAttributeSet.h"

// 데미지 전달 GE. 기본값은 -30.f로 임의 지정
UUNGE_AttackDamage::UUNGE_AttackDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	DefaultDamageAmout = -30.f;

	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UUNCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UUNCharacterAttributeSet, Health)));
	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	FScalableFloat DamageAmout(DefaultDamageAmout);
	FGameplayEffectModifierMagnitude ModMagnitude(DamageAmout);

	HealthModifier.ModifierMagnitude = ModMagnitude;
	Modifiers.Add(HealthModifier);
}
