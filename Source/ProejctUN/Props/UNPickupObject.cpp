// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNPickupObject.h"
#include "Character/UNPlayerCharacter.h"
#include "UI/UNInventoryComponent.h"
#include "Item/ItemBase.h"

AUNPickupObject::AUNPickupObject()
{
	//PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	//SetRootComponent(PickupMesh);
}

void AUNPickupObject::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
}

void AUNPickupObject::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->ItemStatistics = ItemData->ItemStatistics;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;

		ItemReference->NumericData.bIsStackable = ItemData->NumericData.MaxStackSize > 1;
		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		if (ItemData->AssetData.Mesh)
		{
			Mesh->SetStaticMesh(ItemData->AssetData.Mesh);
		}
		else
		{
			SkeletalMesh->SetSkeletalMesh(ItemData->AssetData.SkeletalMesh);
		}

		UpdateInteractableData();
	}
}

void AUNPickupObject::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;

	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	ItemReference->OwningInventory = nullptr;

	if (ItemToDrop->AssetData.Mesh)
	{
		Mesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);
	}

	if (ItemToDrop->AssetData.SkeletalMesh)
	{
		SkeletalMesh->SetSkeletalMesh(ItemToDrop->AssetData.SkeletalMesh);
	}

	UpdateInteractableData();
}

void AUNPickupObject::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void AUNPickupObject::TakePickUp(AActor* Taker)
{
	if (AUNPlayerCharacter* Player = Cast<AUNPlayerCharacter>(Taker))
	{	
		if (!IsPendingKillPending())
		{
			if (ItemReference)
			{
				if (UUNInventoryComponent* PlayerInventory = Player->GetInventoryComponent())
				{
					const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

					switch (AddResult.OperationResult)
					{
					case EItemAddResult::IAR_NoItemAdded:
						break;
					case EItemAddResult::IAR_PartialAmountItemAdded:
						UpdateInteractableData();
						// PlayerCharacter->UpdateInteractionWidget();
						break;
					case EItemAddResult::IAR_AllItemAddeed:
						Destroy();
						break;
					}

					UE_LOG(LogTemp, Log, TEXT("%s"), *AddResult.ResultMessage.ToString());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somehow null!"));
			}
		}
	}
}


// 버그 있을 듯. 체크 꼭 하기!
void AUNPickupObject::Interact(AActor* Player)
{
	bIsSelected = true;
	InteractingActor = Player;
}

void AUNPickupObject::NotifyActorBeginOverlap(AActor* Other)
{
	if (bIsSelected && Other == InteractingActor)
	{
		TakePickUp(Other);
	}
}

void AUNPickupObject::NotifyActorEndOverlap(AActor* Other)
{
	if (Other == InteractingActor)
	{
		bIsSelected = false;
		InteractingActor = nullptr;
	}
}


void AUNPickupObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent)
{
	Super::PostEditChangeProperty(PropertyChangeEvent);

	const FName ChangedPropertyName = PropertyChangeEvent.Property ? PropertyChangeEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AUNPickupObject, DesiredItemID))
	{
		if (ItemDataTable)
		{
			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
			{
				Mesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}