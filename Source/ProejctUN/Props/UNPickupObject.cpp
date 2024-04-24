// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNPickupObject.h"
#include "Character/UNPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Item/ItemBase.h"

AUNPickupObject::AUNPickupObject()
{
	//PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	//SetRootComponent(PickupMesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	Mesh->SetupAttachment(BoxCollision);
	Mesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
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
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		Mesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void AUNPickupObject::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	Mesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);

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
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(Taker);

	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			//if(UInventoryComponent* PlayerInventory  = Taker->GetInventory));
		}
	}
}

void AUNPickupObject::Interact(AActor* Actor)
{
	if (Actor)
	{
		TakePickUp(Actor);
	}
}

void AUNPickupObject::NotifyActorBeginOverlap(AActor* Other)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap"));
}

void AUNPickupObject::NotifyActorEndOverlap(AActor* Other)
{
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