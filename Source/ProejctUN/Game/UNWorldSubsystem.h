// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UNWorldSubsystem.generated.h"

class UDataTable;
class UItemBase;
/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UUNWorldSubsystem();

	UFUNCTION()
	UDataTable* GetItemDataTable() { return ItemDataTable; }

	
	UFUNCTION()
	UItemBase* GetItemReference(FName ItemId);


	//UFUNCTION()
	//FItemData* GetItemReference(FName ItemId);

protected:
	UPROPERTY()
	TObjectPtr<UDataTable> ItemDataTable;
};
