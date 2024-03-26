// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/UNGASWidgetComponent.h"
#include "../UI/UNGASUserWidget.h"

void UUNGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUNGASUserWidget* GASUserWidget = Cast<UUNGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}
