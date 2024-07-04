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

	// �Է� �׸��� �κ��丮�� �̹� �����ϰ� ��ü ������ �ƴ��� Ȯ��
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	// ���� �ִٸ� ���� �ִ� ���ÿ� �л�
	while (ExistingItemStack)
	{
		// ���� ��ü ������ ����� ���� �ʿ��� ���� �׸� ���� ���
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		//���� �뷮�� ���� ������ ����� �� �ִ� "MountToMakeFullStack"�� ���� ���
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// �������� �ܿ����� ���� �뷮�� �ʰ����� ���� ��
		if (WeightLimitAddAmount > 0)
		{
			// ���� �������� ���� ���� �� ���� �߷� ����
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount;

			// ���� ������ ���� ����
			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			// ������ �ִ� ���� ���� �� ���� ����
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
				// ���� ���ÿ� �л� �� �������� ������Ű�� �ش� ��� ���� �߿� ���� ���ѿ� ������ ���
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}

		if (AmountToDistribute <= 0)
		{
			// ��� �л��� �Ϸ���� ��
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// �ٸ� ������ �ִ��� Ȯ��(A�������� �ִ� ��ø�� 64�ε� A(55), A(11) �̷� ������ �� �ִ� ���)
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// �κ��丮�� ���� �������� ���� ��, ���ο� �׸��� ���� �� �ִ� �� Ȯ��
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		// ������ ���� �� ���� ���Կ� �´� ������ �߰� �õ�
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			// ���� ���ѿ� �ɸ� ��� (���� �������� ���Ժ��� ���� ���԰� ���� ���)
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				// ���� �� �ִ� �ִ����� �������� �ִ´�.
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				// ���纻�� �����Ͽ� �߰�
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			// ���� ���԰� ����ϴٸ� ���� �߰�
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
		
		// ������ ������ ���԰� �����ϴٸ�
		return RequestedAddAmount - AmountToDistribute;
	}

	// ������ ���� �� (������ ���õ� ���� �����۵� ����)
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

	// �������� ī�ǵưų�, �ֿ����� ��
	if (Item->bIsCopy || Item->bIsPickup)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	// �ٸ� �κ��丮���� �巡�� ���� ��
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






// �κ��丮 ����ȭ ���� ���� ���� �� . . .
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