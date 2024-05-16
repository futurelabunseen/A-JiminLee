// Fill out your copyright notice in the Description page of Project Settings.


#include "UNCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Tag/UNGameplayTag.h"
#include "Net/UnrealNetwork.h"

// �ʱ�ȭ ����Ʈ�� �ʱ�ȭ
UUNCharacterAttributeSet::UUNCharacterAttributeSet() :
	AttackRange(100.f),
	MaxAttackRange(300.f),
	AttackRadius(50.f),
	AttackRate(30.f),
	MaxAttackRadius(150.f),
	MaxAttackRate(100.f),
	ArmorRate(3.f),
	MaxArmorRate(100.f),
	MaxHealth(100.f),
	Damage(0.f),
	DefaultAttackRange(100.f),
	DefaultAttackRate(30.f),
	DefaultArmorRate(3.f)
{
	// ü�� ����
	InitHealth(GetMaxHealth());
}

void UUNCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.f ? 0.f : NewValue;
	}
}

// ��ũ�θ� ���� ��Ʈ��ũ ȯ�濡�� �������� ������ ����ȭ ���. REPNOTIFY_Always�ɼ��� ���� �׻� �����ǵ��� ����.
void UUNCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, AttackRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, MaxAttackRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, AttackRadius, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, MaxAttackRadius, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, AttackRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, MaxAttackRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, ArmorRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, MaxArmorRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, DefaultAttackRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, DefaultAttackRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUNCharacterAttributeSet, DefaultArmorRate, COND_None, REPNOTIFY_Always);
}

// GE ������ ����Ǹ� bool���� GE ���� ����
bool UUNCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	// ������ ���� GE���� Ȯ��
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// �������� ��ġ(������)�� 0 �̻����� Ȯ��
		if (Data.EvaluatedData.Magnitude > 0.f)
		{
			// �±� Ȯ�� �� �������¶�� �������� 0���� ��ȯ �� false����
			if (Data.Target.HasMatchingGameplayTag(UNTAG_CHARACTER_STATE_INVINSIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.f;
				return false;
			}
		}
	}

	return true;
}

// GE���� ���� ����Ǵ� �Լ�
void UUNCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.f;

	// �ٸ� GE�� ���� ���������� ü���� ����Ǿ��� �� 
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	
	// �������� �Ծ��� ��
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage() + GetArmorRate(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.f);
	}

	// ü���� 0�̵Ǹ� ��������Ʈ ����
	if ((GetHealth() <= 0.f) && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(UNTAG_CHARACTER_STATE_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.f);
}

// AttributeSet�� ����Ǿ��� �� ��ũ�θ� ���� ���� ������ �˸�
void UUNCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, Health, OldHealth);
}

void UUNCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UUNCharacterAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, Damage, OldDamage);
}

void UUNCharacterAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& OldAttackRange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, AttackRange, OldAttackRange);
}

void UUNCharacterAttributeSet::OnRep_MaxAttackRange(const FGameplayAttributeData& OldMaxAttackRange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, MaxAttackRange, OldMaxAttackRange);
}

void UUNCharacterAttributeSet::OnRep_AttackRadius(const FGameplayAttributeData& OldAttackRadius)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, AttackRadius, OldAttackRadius);
}

void UUNCharacterAttributeSet::OnRep_MaxAttackRadius(const FGameplayAttributeData& OldMaxAttackRadius)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, MaxAttackRadius, OldMaxAttackRadius);
}

void UUNCharacterAttributeSet::OnRep_AttackRate(const FGameplayAttributeData& OldAttackRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, AttackRate, OldAttackRate);
}

void UUNCharacterAttributeSet::OnRep_MaxAttackRate(const FGameplayAttributeData& OldMaxAttackRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, MaxAttackRate, OldMaxAttackRate);
}

void UUNCharacterAttributeSet::OnRep_ArmorRate(const FGameplayAttributeData& OldArmorRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, ArmorRate, OldArmorRate);
}

void UUNCharacterAttributeSet::OnRep_MaxArmorRate(const FGameplayAttributeData& OldMaxArmorRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, MaxArmorRate, OldMaxArmorRate);
}

void UUNCharacterAttributeSet::OnRep_DefaultAttackRange(const FGameplayAttributeData& OldDefaultAttackRange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, DefaultAttackRange, OldDefaultAttackRange);
}

void UUNCharacterAttributeSet::OnRep_DefaultAttackRate(const FGameplayAttributeData& OldDefaultAttackRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, DefaultAttackRate, OldDefaultAttackRate);
}

void UUNCharacterAttributeSet::OnRep_DefaultArmorRate(const FGameplayAttributeData& OldDefaultArmorRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUNCharacterAttributeSet, DefaultArmorRate, OldDefaultArmorRate);
}
