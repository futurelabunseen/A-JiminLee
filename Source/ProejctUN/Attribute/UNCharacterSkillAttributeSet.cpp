// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/UNCharacterSkillAttributeSet.h"

UUNCharacterSkillAttributeSet::UUNCharacterSkillAttributeSet() :
	SKillRange(800.f),
	MaxSkillRange(1200.f),
	SKillRate(150.f),
	MaxSkillRate(300.f),
	SKillEnergy(100.f),
	MaxSkillEnergy(100.f)
{
}

void UUNCharacterSkillAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetSKillRangeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, GetMaxSkillRange());
	}
	else if (Attribute == GetSKillRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSkillRate());
	}
}
