// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UNInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROEJCTUN_API UUNInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUNInventoryComponent();

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
