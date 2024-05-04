// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UNInventoryItemSlotWidget.h"
#include "UNInventoryEquipSlot.generated.h"

class AUNPlayerCharacter;
class UUNInventoryTooltip;
class UUNDragItemVisual;
class UItemBase;

class UBorder;
class UImage;
class UTextBlock;
class UTexture2D;

/**
 * 
 */


UCLASS()
class PROEJCTUN_API UUNInventoryEquipSlot : public UUNInventoryItemSlotWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AUNPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere)
	FLinearColor DefaultBorderColor;

	UPROPERTY(EditAnywhere)
	UTexture2D* DefaultIconImage;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void ClearSlotData();
};
