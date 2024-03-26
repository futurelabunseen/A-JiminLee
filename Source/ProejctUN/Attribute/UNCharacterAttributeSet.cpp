// Fill out your copyright notice in the Description page of Project Settings.


#include "../Attribute/UNCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "../Tag/UNGameplayTag.h"

UUNCharacterAttributeSet::UUNCharacterAttributeSet() :
	AttackRange(100.f),
	MaxAttackRange(300.f),
	AttackRadius(50.f),
	AttackRate(30.f),
	MaxAttackRadius(150.f),
	MaxAttackRate(100.f),
	MaxHealth(100.f),
	Damage(0.f)
{
	InitHealth(GetMaxHealth());
}

void UUNCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//if (Attribute == GetHealthAttribute())
	//{
	//	NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	//}

	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.f ? 0.f : NewValue;
	}
}

bool UUNCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.f)
		{
			if (Data.Target.HasMatchingGameplayTag(UNTAG_CHARACTER_STATE_INVINSIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.f;
				return false;
			}
		}
	}

	return true;
}

void UUNCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}

	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.f);
	}

	if ((GetHealth() <= 0.f) && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(UNTAG_CHARACTER_STATE_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.f);
}

//void UUNCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
//{
//	if (Attribute == GetHealthAttribute())
//	{
//		UE_LOG(LogTemp, Log, TEXT("%f -> %f"), OldValue, NewValue);
//	}
//}
