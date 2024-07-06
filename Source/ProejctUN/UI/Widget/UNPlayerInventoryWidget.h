// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UNGASUserWidget.h"
#include "UNPlayerInventoryWidget.generated.h"

class AUNPlayerCharacter;
class UUNInventoryComponent;
class UUNInventoryItemSlotWidget;

class UWrapBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNPlayerInventoryWidget : public UUNGASUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void RefreshInventory();

	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryWrapBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeightInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CapacityInfo;

	UPROPERTY()
	AUNPlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UUNInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUNInventoryItemSlotWidget> InventorySlotClass;

protected:
	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	UUNInventoryItemSlotWidget* GetEmptyItemSlot();
};
