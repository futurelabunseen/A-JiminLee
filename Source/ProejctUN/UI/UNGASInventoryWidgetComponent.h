// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "UNGASInventoryWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNGASInventoryWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "Variable")
	bool Inventory;

	UPROPERTY(EditAnywhere, Category = "Variable")
	int32 MaxSize;

	UPROPERTY(EditAnywhere, Category = "Variable")
	bool AllowDuplicates;

	UPROPERTY(EditAnywhere, Category = "Variable")
	bool AsUMGInventory;
};
