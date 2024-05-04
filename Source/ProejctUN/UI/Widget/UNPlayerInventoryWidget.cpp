// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNPlayerInventoryWidget.h"
#include "UI/Widget/UNInventoryItemSlotWidget.h"
#include "Character/UNPlayerCharacter.h"
#include "UI/UNInventoryComponent.h"
#include "UI/UNItemDragDropOperation.h"

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
	/*WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		InventoryReference->GetInventoryTotalWeight(),
		InventoryReference->GetWeightCapacity()));
	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		InventoryReference->GetInventoryContents().Num(),
		InventoryReference->GetSlotsCapacity()));*/

	const FString WeightInfoValue{ FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/"
	+ FString::SanitizeFloat(InventoryReference->GetWeightCapacity()) };

	const FString CapacityInfoValue{ FString::SanitizeFloat(InventoryReference->GetInventoryContents().Num()) + "/"
	+ FString::SanitizeFloat(InventoryReference->GetSlotsCapacity()) };

	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}

void UUNPlayerInventoryWidget::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryWrapBox->ClearChildren();

		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UUNInventoryItemSlotWidget* ItemSlot = CreateWidget<UUNInventoryItemSlotWidget>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}
}

bool UUNPlayerInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	const UUNItemDragDropOperation* ItemDragDrop = Cast<UUNItemDragDropOperation>(InOperation);
	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on inventoryPanel."));

		// 드롭 중지
		return true;
	}
	
	// 드롭 진행
	return false;
}
