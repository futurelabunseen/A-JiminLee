// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UNPlayerCharacter.h"
#include "Struct/ItemDataStructs.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UItemBase : public UObject
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY()
	UUNInventoryComponent* OwningInventory;

	UPROPERTY(VisibleAnywhere, Category = "Item Data", meta = (UIMin=1, UIMax=100))
	int32 Quantity;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;
	
	UItemBase();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; }
	
	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(AUNPlayerCharacter* Character);

protected:
	bool operator==(const FName& OtherID) const
	{
		return ID == OtherID;
	}
};
