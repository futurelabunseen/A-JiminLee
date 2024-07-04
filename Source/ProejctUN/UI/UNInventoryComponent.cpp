// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UNInventoryComponent.h"
#include "Item/ItemBase.h"
#include "Game/UNWorldSubsystem.h"
#include "Net/UnrealNetwork.h"
//#include "Character/UNPlayerCharacter.h"

// Sets default values for this component's properties
UUNInventoryComponent::UUNInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UUNInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUNInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UUNInventoryComponent, InventoryItemIDArray);
	DOREPLIFETIME(UUNInventoryComponent, CurrentWeaponItemID);
	DOREPLIFETIME(UUNInventoryComponent, CurrentArmorItemID);
}

UItemBase* UUNInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UUNInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UUNInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
			{
				return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
			}
		))
	{
		return *Result;
	}

	return nullptr;
}

int32 UUNInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UUNInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UUNInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UUNInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiresAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiresAmountToRemove, ItemIn->Quantity);
	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	//ServerRPCRemoveItem(ItemIn->ID, DesiresAmountToRemove);

	OnInventoryUpdated.Broadcast();
	
	return ActualAmountToRemove;
}

void UUNInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UUNInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."), InputItem->TextData.Name));
	}

	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), InputItem->TextData.Name));
	}

	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Could not add {0} to the inventory. All inventory slots are full"), InputItem->TextData.Name));
	}

	AddNewItem(InputItem, 1);

	return FItemAddResult::AddedAll(1, FText::Format(
		FText::FromString("Successfully add {0} to the inventory."), InputItem->TextData.Name));
}

int32 UUNInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		// invalid item data
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;

	// 입력 항목이 인벤토리에 이미 존재하고 전체 스택이 아닌지 확인
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	// 만약 있다면 남아 있는 스택에 분산
	while (ExistingItemStack)
	{
		// 다음 전체 스택을 만들기 위해 필요한 기존 항목 수를 계산
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		//무게 용량에 따라 실제로 운반할 수 있는 "MountToMakeFullStack"의 수를 계산
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// 아이템의 잔여량이 무게 용량을 초과하지 않을 때
		if (WeightLimitAddAmount > 0)
		{
			// 기존 아이템의 스택 수량 및 무게 중량 수정
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount;

			// 남은 아이템 갯수 조절
			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			// 가방의 최대 무게 도달 시 루프 종료
			if (InventoryTotalWeight + ExistingItemStack->GetItemSingleWeight() > InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if(WeightLimitAddAmount <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				// 여러 스택에 분산 및 아이템을 증가시키고 해당 기능 수행 중에 무게 제한에 도달할 경우
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}

		if (AmountToDistribute <= 0)
		{
			// 모든 분산이 완료됐을 때
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// 다른 스택이 있는지 확인(A아이템의 최대 중첩이 64인데 A(55), A(11) 이런 식으로 들어가 있는 경우)
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// 인벤토리에 같은 아이템이 없을 때, 새로운 항목을 만들 수 있는 지 확인
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		// 공간이 있을 시 남은 무게에 맞는 아이템 추가 시도
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			// 무게 제한에 걸릴 경우 (남은 아이템의 무게보다 가방 무게가 적을 경우)
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				// 넣을 수 있는 최대한의 아이템을 넣는다.
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				// 복사본을 생성하여 추가
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			// 남은 무게가 충분하다면 전부 추가
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
		
		// 공간은 있지만 무게가 무족하다면
		return RequestedAddAmount - AmountToDistribute;
	}

	// 공간이 없을 때 (기존에 스택된 같은 아이템도 없음)
	return 0;
}

FItemAddResult UUNInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		if(!InputItem->NumericData.bIsStackable)
		{	
			return HandleNonStackableItems(InputItem);
		}

		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(StackableAmountAdded, FText::Format(
				FText::FromString("Successfully add {0} {1} to the inventory."), StackableAmountAdded, InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"), InputItem->TextData.Name, StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."), InputItem->TextData.Name));
		}

	}
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed"));
}

void UUNInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	// 아이템이 카피됐거나, 주워졌을 때
	if (Item->bIsCopy || Item->bIsPickup)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	// 다른 인벤토리에서 드래그 됐을 때
	else
	{
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	//FRPCItemData ItemData;
	//ItemData.Quantity = NewItem->Quantity;
	//ItemData.ID = NewItem->ID;
	//ItemData.ItemType = NewItem->ItemType;
	//ItemData.ItemQuality = NewItem->ItemQuality;
	//ItemData.ItemStatistics = NewItem->ItemStatistics;
	//ItemData.TextData = NewItem->TextData;
	//ItemData.NumericData = NewItem->NumericData;
	//ItemData.AssetData = NewItem->AssetData;
	//ItemData.bIsCopy = NewItem->bIsCopy;
	//ItemData.bIsPickup = NewItem->bIsPickup;
	//ItemData.bIsEquip = NewItem->bIsEquip;

	//AddInventoryItemID(NewItem->ID);
	//ServerRPCAddItem(NewItem);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}

void UUNInventoryComponent::AddInventoryItemID_Implementation(FName ItemID)
{
	//InventoryItemIDArray.Add(ItemID);
}

void UUNInventoryComponent::RemoveInventoryItemID_Implementation(FName ItemID)
{

}

void UUNInventoryComponent::AddWeaponItemID_Implementation(FName ItemID)
{
	CurrentWeaponItemID = ItemID;
}

void UUNInventoryComponent::RemoveWeaponItemID_Implementation()
{
	CurrentWeaponItemID = NAME_None;
}

void UUNInventoryComponent::AddArmorItemID_Implementation(FName ItemID)
{
	CurrentArmorItemID = ItemID;
}

void UUNInventoryComponent::RemoveArmorItemID_Implementation()
{
	CurrentArmorItemID = NAME_None;
}






// 인벤토리 동기화 관련 수정 진행 중 . . .
// ======================================================================================================
// ======================================================================================================

//bool UUNInventoryComponent::ServerRPCAddItem_Validate(const FRPCItemData& RPCItem)
//{
//	return true;
//}

void UUNInventoryComponent::ServerRPCAddItem_Implementation(UItemBase* Item)
{
	//UE_LOG(LogTemp, Log, TEXT("%s"), Item->ID);
	
	//UItemBase* ItemReference = NewObject<UItemBase>(this, UItemBase::StaticClass());

	//ItemReference->ID = RPCItem.ID;
	//ItemReference->ItemType = RPCItem.ItemType;
	//ItemReference->ItemQuality = RPCItem.ItemQuality;
	//ItemReference->ItemStatistics = RPCItem.ItemStatistics;
	//ItemReference->NumericData = RPCItem.NumericData;
	//ItemReference->TextData = RPCItem.TextData;
	//ItemReference->AssetData = RPCItem.AssetData;
	//ItemReference->bIsCopy = RPCItem.bIsCopy;
	//ItemReference->bIsEquip = RPCItem.bIsEquip;
	//ItemReference->bIsPickup = RPCItem.bIsPickup;
	//ItemReference->OwningInventory = this;

	//InventoryContents.Add(ItemReference);
	//InventoryTotalWeight += ItemReference->GetItemStackWeight();
	//
	
	
	
	
	//ClientRPCInvenUpdate();
}

void UUNInventoryComponent::ServerRPCRemoveItem_Implementation(FName ItemId, int32 DesiresAmountToRemove)
{
	//UE_LOG(LogTemp, Log, TEXT("%s"), ItemId);

	//for (const auto& InventoryContent : InventoryContents)
	//{
	//	if (InventoryContent->ID == ItemId)
	//	{
	//		const int32 ActualAmountToRemove = FMath::Min(DesiresAmountToRemove, InventoryContent->Quantity);
	//		InventoryContent->SetQuantity(InventoryContent->Quantity - ActualAmountToRemove);

	//		InventoryTotalWeight -= ActualAmountToRemove * InventoryContent->GetItemSingleWeight();
	//	}
	//}
}

void UUNInventoryComponent::ClientRPCInvenUpdate_Implementation()
{
	//AUNPlayerCharacter* Player = Cast<AUNPlayerCharacter>(GetOwner());
	//if (Player->IsLocallyControlled())
	//{
	//	OnInventoryUpdated.Broadcast();
	//}
}



void UUNInventoryComponent::OnRep_InventoryContents()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "2");
	//UE_LOG(LogTemp, Log, TEXT("Hi"));
	//ClientRPCInvenUpdate();
}

// ======================================================================================================
// ======================================================================================================






void UUNInventoryComponent::EquipItem(UItemBase* ItemIn)
{
	switch (ItemIn->ItemType)
	{
	case EItemType::Weapon:
		if (WeaponSlot != nullptr)
		{
			WeaponSlot->bIsEquip = false;
			HandleAddItem(WeaponSlot);
		}
		ItemIn->bIsEquip = true;
		WeaponSlot = ItemIn;
		AddWeaponItemID(ItemIn->ID);
		RemoveAmountOfItem(ItemIn, 1);
		break;
	case EItemType::Armor:
		if (ArmorSlot != nullptr)
		{
			ArmorSlot->bIsEquip = false;
			HandleAddItem(ArmorSlot);
		}
		ItemIn->bIsEquip = true;
		ArmorSlot = ItemIn;
		AddArmorItemID(ItemIn->ID);
		RemoveAmountOfItem(ItemIn, 1);
		break;
	default:
		break;
	}
}

void UUNInventoryComponent::UnEquipItem(UItemBase* ItemIn)
{
	switch (ItemIn->ItemType)
	{
	case EItemType::Weapon:
		ItemIn->bIsEquip = false;
		WeaponSlot = nullptr;
		RemoveWeaponItemID();
		HandleAddItem(ItemIn);
		break;
	case EItemType::Armor:
		ItemIn->bIsEquip = false;
		ArmorSlot = nullptr;
		RemoveArmorItemID();
		HandleAddItem(ItemIn);
		break;
	default:
		break;
	}
}