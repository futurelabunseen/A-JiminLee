// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UNWorldSubsystem.h"
#include "Struct/ItemDataStructs.h"
#include "Item/ItemBase.h"

UUNWorldSubsystem::UUNWorldSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/TestItems2.TestItems2'"));
	if (nullptr != ItemDataTableRef.Object)
	{
		ItemDataTable = ItemDataTableRef.Object;
	}
}

UItemBase* UUNWorldSubsystem::GetItemReference(FName ItemId)
{
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemId, ItemId.ToString());

	UItemBase* ItemReference = NewObject<UItemBase>(this, UItemBase::StaticClass());

	ItemReference->ID = ItemData->ID;
	ItemReference->ItemType = ItemData->ItemType;
	ItemReference->ItemQuality = ItemData->ItemQuality;
	ItemReference->ItemStatistics = ItemData->ItemStatistics;
	ItemReference->NumericData = ItemData->NumericData;
	ItemReference->TextData = ItemData->TextData;
	ItemReference->AssetData = ItemData->AssetData;

	return ItemReference;
}
