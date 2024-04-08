// Fill out your copyright notice in the Description page of Project Settings.


#include "UNGASWidgetComponent.h"
#include "UNGASUserWidget.h"

void UUNGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUNGASUserWidget* GASUserWidget = Cast<UUNGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}