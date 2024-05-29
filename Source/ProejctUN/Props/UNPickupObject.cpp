// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNPickupObject.h"
#include "Character/UNPlayerCharacter.h"
#include "UI/UNInventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Item/ItemBase.h"
#include "Net/UnrealNetwork.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "Engine/DataTable.h"
#include "Math/UnrealMathUtility.h"

AUNPickupObject::AUNPickupObject()
{
	//PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	//SetRootComponent(PickupMesh);

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/TestItems2.TestItems2'"));
	if (nullptr != DataTableRef.Object)
	{
		ItemDataTable = DataTableRef.Object;
	}

	BoxCollision->SetBoxExtent(FVector(150.f, 150.f, 1.f));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AUNPickupObject::OnBoxCollisionBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AUNPickupObject::OnBoxCollisionEndOverlap);
	bReplicates = true;
}

void AUNPickupObject::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePickup(UItemBase::StaticClass(), ItemQuantity);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				SkeletalMesh->SetSimulatePhysics(false);
			}, 5.0f, false);
	}
}

void AUNPickupObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUNPickupObject, DesiredItemID);
}

void AUNPickupObject::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (DesiredItemID.IsNone())
	{
		TArray<FName> ItemNames = ItemDataTable->GetRowNames();
		const int32 NumRows = ItemNames.Num();
		//const int32 NumRows = ItemDataTable->GetTableData().Num();
		const int32 RandomRowIndex = FMath::RandRange(0, NumRows - 2);

		const TArray<FName>& RowNames = ItemDataTable->GetRowNames();
		DesiredItemID = RowNames[RandomRowIndex];
		ItemQuantity = 1;
		//ClientRPCSetRandomValue(DesiredItemID);
	}

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
			Mesh->SetSimulatePhysics(true);
			Mesh->SetCollisionProfileName("UNPickUpObject");
			BoxCollision->SetCollisionProfileName("OverlapOnlyPawn");
		}
		else
		{
			SkeletalMesh->SetSkeletalMesh(ItemData->AssetData.SkeletalMesh);
			SkeletalMesh->SetSimulatePhysics(true);
			SkeletalMesh->SetCollisionProfileName("UNPickUpObject");
			BoxCollision->SetCollisionProfileName("OverlapOnlyPawn");
		}

		UpdateInteractableData();
	}
}

void AUNPickupObject::OnRep_DesiredItemID()
{
	InitializePickup(UItemBase::StaticClass(), 1);
}

void AUNPickupObject::InitializeDropItem(FName ID, int32 Quantity)
{
	DesiredItemID = ID;
	ItemQuantity = Quantity;
	InitializePickup(UItemBase::StaticClass(), Quantity);
}

void AUNPickupObject::ServerRPCDestoryActor_Implementation()
{
	this->Destroy(true);
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
						Player->ServerRPCDestoryActor(this);
						//ServerRPCDestoryActor();
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

void AUNPickupObject::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsSelected && OtherActor == InteractingActor)
	{
		TakePickUp(OtherActor);
	}
}

void AUNPickupObject::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == InteractingActor)
	{
		EndInteract();
	}
}

void AUNPickupObject::Interact(AActor* Player)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, "Interact");
	bIsSelected = true;
	InteractingActor = Player;
}

void AUNPickupObject::EndInteract()
{
	bIsSelected = false;
	InteractingActor = nullptr;
}

void AUNPickupObject::NotifyActorBeginOverlap(AActor* Other)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, "BeginOverlap");
	//if (bIsSelected && Other == InteractingActor)
	//{
	//	TakePickUp(Other);
	//}
}

void AUNPickupObject::NotifyActorEndOverlap(AActor* Other)
{
	//if (Other == InteractingActor)
	//{
	//	EndInteract();
	//}
}


//void AUNPickupObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangeEvent);
//
//	Mesh->SetStaticMesh(nullptr);
//	SkeletalMesh->SetSkeletalMesh(nullptr);
//
//	const FName ChangedPropertyName = PropertyChangeEvent.Property ? PropertyChangeEvent.Property->GetFName() : NAME_None;
//
//	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AUNPickupObject, DesiredItemID))
//	{
//		if (ItemDataTable)
//		{
//			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
//			{
//				Mesh->SetStaticMesh(ItemData->AssetData.Mesh);
//				SkeletalMesh->SetSkeletalMesh(ItemData->AssetData.SkeletalMesh);
//			}
//		}
//	}
//}