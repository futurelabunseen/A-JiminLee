// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNInventoryEquipSlot.h"
#include "Item/ItemBase.h"
#include "UI/Widget/UNDragItemVisual.h"
#include "UI/UNItemDragDropOperation.h"
#include "Character/UNPlayerCharacter.h"
#include "UI/UNInventoryComponent.h"
#include "UI/Widget/UNInventoryTooltip.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUNInventoryEquipSlot::NativeOnInitialized()
{
	//Super::NativeOnInitialized();
	ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	ClearSlotData();

	PlayerCharacter = Cast<AUNPlayerCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventoryComponent();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UUNInventoryEquipSlot::UpdateSlotData);
		}
	}
}

void UUNInventoryEquipSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUNInventoryEquipSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp, Log, TEXT("detect"));
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass)
	{
		const TObjectPtr<UUNDragItemVisual> DragVisual = CreateWidget<UUNDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		ItemReference->NumericData.bIsStackable ? DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity)) : DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

		UUNItemDragDropOperation* DragItemOperation = NewObject<UUNItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;
	}
}

bool UUNInventoryEquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UUNItemDragDropOperation* ItemDragDrop = Cast<UUNItemDragDropOperation>(InOperation);
	if (ItemDragDrop)
	{
		ItemReference = ItemDragDrop->SourceItem;
		UpdateSlotData();

		return true;
	}

	return false;
}

void UUNInventoryEquipSlot::ClearSlotData()
{
	ItemBorder->SetBrushColor(DefaultBorderColor);
	ItemIcon->SetBrushFromTexture(DefaultIconImage);
}


void UUNInventoryEquipSlot::UpdateSlotData()
{
	if (SlotData == ESlotData::Weapon)
	{
		ItemReference = InventoryReference->WeaponSlot;
	}
	else if (SlotData == ESlotData::Armor)
	{
		ItemReference = InventoryReference->ArmorSlot;
	}

	if (ItemReference)
	{
		UpdateSlot();
		CreateToolTip();
	}
	else
	{
		ClearSlotData();
	}
}
