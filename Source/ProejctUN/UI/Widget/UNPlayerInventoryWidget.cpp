// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNPlayerInventoryWidget.h"
#include "UI/Widget/UNInventoryItemSlotWidget.h"
#include "Character/UNPlayerCharacter.h"
#include "UI/UNInventoryComponent.h"

#include "Components/WrapBox.h"
#include "Components/TextBlock.h"

void UUNPlayerInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AUNPlayerCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventoryComponent();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UUNPlayerInventoryWidget::RefreshInventory);
			SetInfoText();
		}
	}
}

void UUNPlayerInventoryWidget::SetInfoText() const
{
	WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		InventoryReference->GetInventoryTotalWeight(),
		InventoryReference->GetWeightCapacity()));
	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		InventoryReference->GetInventoryContents().Num(),
		InventoryReference->GetSlotsCapacity()));

}

void UUNPlayerInventoryWidget::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UUNInventoryItemSlotWidget* ItemSlot = CreateWidget<UUNInventoryItemSlotWidget>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
	}
}

bool UUNPlayerInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//Super::NativeOnDrop();
	return false;
}
