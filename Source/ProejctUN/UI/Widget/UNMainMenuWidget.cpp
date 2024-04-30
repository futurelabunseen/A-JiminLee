// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNMainMenuWidget.h"
#include "Character/UNPlayerCharacter.h"

void UUNMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUNMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AUNPlayerCharacter>(GetOwningPlayerPawn());
}

bool UUNMainMenuWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
