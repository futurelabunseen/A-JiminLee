// Fill out your copyright notice in the Description page of Project Settings.


#include "../Attribute/UNCharacterAttributeSet.h"

UUNCharacterAttributeSet::UUNCharacterAttributeSet() :
	AttackRange(100.f),
	MaxAttackRange(300.f),
	AttackRadius(50.f),
	AttackRate(30.f),
	MaxAttackRadius(150.f),
	MaxAttackRate(100.f),
	MaxHealth(100.f)
{
	InitHealth(GetMaxHealth());
}

void UUNCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UUNCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("%f -> %f"), OldValue, NewValue);
	}
}
