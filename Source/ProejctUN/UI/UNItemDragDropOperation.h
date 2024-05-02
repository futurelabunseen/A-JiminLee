// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "UNItemDragDropOperation.generated.h"

class UItemBase;
class UUNInventoryComponent;
/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UItemBase* SourceItem;

	UPROPERTY()
	UUNInventoryComponent* SourceInventory;
};
