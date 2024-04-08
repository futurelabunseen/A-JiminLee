// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UNGASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UUNGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UUNGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
