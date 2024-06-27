// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNMainMenuWidget.h"
#include "UI/UNItemDragDropOperation.h"
#include "Item/ItemBase.h"

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
	const UUNItemDragDropOperation* ItemDragDrop = Cast<UUNItemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;
}
