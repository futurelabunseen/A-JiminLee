// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/ItemDataStructs.h"
#include "UNInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAddeed UMETA(DisplayName = "All of item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()
	
	FItemAddResult() :
		ActualAmountAdded(0),
		OperationResult(EItemAddResult::IAR_NoItemAdded),
		ResultMessage(FText::GetEmpty())
	{};
	
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	}

	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAddeed;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	}

};

USTRUCT(BlueprintType)
struct FRPCItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemQuality ItemQuality;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData AssetData;

	UPROPERTY()
	bool bIsCopy;

	UPROPERTY()
	bool bIsPickup;

	UPROPERTY()
	bool bIsEquip;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROEJCTUN_API UUNInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnInventoryUpdated OnInventoryUpdated;

	// Sets default values for this component's properties
	UUNInventoryComponent();
	
	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* InputItem);

	UFUNCTION(Category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;
	

	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);

	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiresAmountToRemove);

	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit);


	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight;  };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; };


	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const int32 NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };
	///////////
	void EquipItem(UItemBase* ItemIn);
	void UnEquipItem(UItemBase* ItemIn);

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	UItemBase* WeaponSlot;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	UItemBase* ArmorSlot;

	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRPCAddItem(const FRPCItemData& RPCItem);

	UFUNCTION(Server, unreliable)
	void ServerRPCAddItem(UItemBase* Item);

	UFUNCTION(Server, Unreliable)
	void ServerRPCRemoveItem(FName ItemId, int32 DesiresAmountToRemove);


	UFUNCTION(Client, Unreliable)
	void ClientRPCInvenUpdate();

	UFUNCTION()
	void OnRep_InventoryContents();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	float InventoryWeightCapacity;

	//UPROPERTY(ReplicatedUsing = OnRep_InventoryContents, VisibleAnywhere, Category = "Inventory")
	//TArray<TObjectPtr<UItemBase>> InventoryContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;

	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UItemBase* InputItem);
	int32 HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);

	//UPROPERTY(EditAnywhere, Category = "Variable")
	//bool Inventory;

	//UPROPERTY(EditAnywhere, Category = "Variable")
	//int32 MaxSize;

	//UPROPERTY(EditAnywhere, Category = "Variable")
	//bool AllowDuplicates;

	//UPROPERTY(EditAnywhere, Category = "Variable")
	//bool AsUMGInventory;
};
