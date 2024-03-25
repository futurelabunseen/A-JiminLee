// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/UNGASHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "../Attribute/UNCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUNGASHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UUNCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UUNGASHpBarUserWidget::OnHealthChange);
		ASC->GetGameplayAttributeValueChangeDelegate(UUNCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UUNGASHpBarUserWidget::OnMaxHealthChange);

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
}

void UUNGASHpBarUserWidget::OnHealthChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UUNGASHpBarUserWidget::OnMaxHealthChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

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
}
