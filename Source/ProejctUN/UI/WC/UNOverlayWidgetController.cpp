// Fill out your copyright notice in the Description page of Project Settings.


#include "UNOverlayWidgetController.h"
#include "Attribute/UNCharacterAttributeSet.h"

void UUNOverlayWidgetController::BroadcastInitialValues()
{
	//const UUNCharacterAttributeSet* UNCharacterAttributeSet = CastChecked<UUNCharacterAttributeSet>(AttributeSet);

	//OnHealthChanged.Broadcast(UNCharacterAttributeSet->GetHealth());
	//OnMaxHealthChanged.Broadcast(UNCharacterAttributeSet->GetMaxHealth());
}

void UUNOverlayWidgetController::BindCallbacksToDependencies()
{/*
	const UUNCharacterAttributeSet* UNCharacterAttributeSet = CastChecked<UUNCharacterAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UNCharacterAttributeSet->GetHealthAttribute()).AddUObject(this, &UUNOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UNCharacterAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UUNOverlayWidgetController::MaxHealthChanged);*/
}

//void UUNOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
//{
//	//OnHealthChanged.Broadcast(Data.NewValue);
//}
//
//void UUNOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
//{
//	//OnMaxHealthChanged.Broadcast(Data.NewValue);
//}
