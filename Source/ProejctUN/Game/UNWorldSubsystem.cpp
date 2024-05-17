// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UNWorldSubsystem.h"
#include "Struct/ItemDataStructs.h"

UUNWorldSubsystem::UUNWorldSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/TestItems2.TestItems2'"));
	if (nullptr != ItemDataTableRef.Object)
	{
		ItemDataTable = ItemDataTableRef.Object;
	}
}

//FItemData* UUNWorldSubsystem::GetItemReference(FName ItemId)
//{
//	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemId, ItemId.ToString());
//	return ItemData;
//}
