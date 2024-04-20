// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNGASHpBarUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/UNPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Attribute/UNCharacterAttributeSet.h"
#include "Tag/UNGameplayTag.h"

// ASC�� HPBar���� ��������Ʈ ���� �� �ʱ�ȭ 
void UUNGASHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UUNCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UUNGASHpBarUserWidget::OnHealthChange);
		ASC->GetGameplayAttributeValueChangeDelegate(UUNCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UUNGASHpBarUserWidget::OnMaxHealthChange);
		ASC->RegisterGameplayTagEvent(UNTAG_CHARACTER_STATE_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UUNGASHpBarUserWidget::OnInvinsibleTagChange);

		PbHpBar->SetFillColorAndOpacity(HealthColor);

		const UUNCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UUNCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
			if (CurrentHealth > 0.f)
			{
				UpdateHpBar();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Not Have ASC!"));
	}
}

// ü���� ��ȭ
void UUNGASHpBarUserWidget::OnHealthChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

// �ִ� ü���� ��ȭ
void UUNGASHpBarUserWidget::OnMaxHealthChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

// ���� �±װ� ��ȭ
void UUNGASHpBarUserWidget::OnInvinsibleTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvinsibleColor);
		PbHpBar->SetPercent(1.f);
	}
	else
	{
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHpBar();
	}
}

// ����� Attribute�� Hpbar ������Ʈ
void UUNGASHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
	
	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), CurrentHealth, CurrentMaxHealth)));
	}

	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(ASC->GetOwnerActor());
	UE_LOG(LogTemp, Log, TEXT("Test : %s"), *ASC->GetOwnerActor()->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("HpBarUpdate"));
}