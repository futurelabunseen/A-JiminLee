// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/UNInteractableObjectBase.h"
#include "UNPickupObject.generated.h"

class UItemBase;
class UDataTable;
class AUNPlayerCharacter;

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNPickupObject : public AUNInteractableObjectBase
{
	GENERATED_BODY()

public:
	AUNPickupObject();

	virtual void BeginPlay() override;
	virtual void Interact(AActor* Player) override;

	virtual void NotifyActorBeginOverlap(class AActor* Other) override;
	virtual void NotifyActorEndOverlap(class AActor* Other) override;

	void UpdateInteractableData();
	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);
	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);
	void TakePickUp(AActor* Taker);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	bool bIsSelected;

	UPROPERTY()
	AActor* InteractingActor;

//
//#if WITH_EDITOR
//	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) override;
//#endif
};
