// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UNInventoryItemSlotWidget.h"
#include "UNInventoryEquipSlot.generated.h"

class AUNPlayerCharacter;
class UUNInventoryComponent;
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
UENUM()
enum class ESlotData : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor")
};

UCLASS()
class PROEJCTUN_API UUNInventoryEquipSlot : public UUNInventoryItemSlotWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AUNPlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UUNInventoryComponent* InventoryReference;

	UPROPERTY(EditAnywhere)
	FLinearColor DefaultBorderColor;

	UPROPERTY(EditAnywhere)
	UTexture2D* DefaultIconImage;

	UPROPERTY(EditAnywhere)
	ESlotData SlotData;

	UFUNCTION()
	void UpdateSlotData();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void ClearSlotData();
};
