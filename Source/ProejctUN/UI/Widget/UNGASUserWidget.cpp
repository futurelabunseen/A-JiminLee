// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNGASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UUNGASUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

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
