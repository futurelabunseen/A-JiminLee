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

	BoxCollision->SetBoxExtent(FVector(150.f, 150.f, 150.f));
	BoxCollision->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECollisionResponse::ECR_Ignore);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AUNPickupObject::OnBoxCollisionBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AUNPickupObject::OnBoxCollisionEndOverlap);

	bReplicates = true;
}

void AUNPickupObject::BeginPlay()
{
	Super::BeginPlay();
		
	InitializePickup(UItemBase::StaticClass(), ItemQuantity);

	MoveToFloor();
}

void AUNPickupObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUNPickupObject, DesiredItemID);
}

void AUNPickupObject::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	// 회전값 랜덤
	float RandomRotation = FMath::RandRange(0, 360);
	float RandomLookAt = (FMath::RandRange(0, 1)) == 0 ? -90.f : 90.f;

	this->SetActorRotation(FRotator(RandomLookAt, RandomRotation, 0.f));

	// 데이터 테이블에 랜덤 ID반환
	if (DesiredItemID.IsNone())
	{
		TArray<FName> ItemNames = ItemDataTable->GetRowNames();
		const int32 NumRows = ItemNames.Num();
		const int32 RandomRowIndex = FMath::RandRange(0, NumRows - 1);

		const TArray<FName>& RowNames = ItemDataTable->GetRowNames();
		DesiredItemID = RowNames[RandomRowIndex];

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
			Mesh->SetCollisionProfileName("UNPickUpObject");
			Mesh->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECollisionResponse::ECR_Ignore);
			BoxCollision->SetCollisionProfileName("ItemTrigger");
		}
		else
		{
			SkeletalMesh->SetSkeletalMesh(ItemData->AssetData.SkeletalMesh);
			SkeletalMesh->SetCollisionProfileName("UNPickUpObject");
			SkeletalMesh->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECollisionResponse::ECR_Ignore);
			BoxCollision->SetCollisionProfileName("ItemTrigger");
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

void AUNPickupObject::ServerRPCDestroyActor_Implementation()
{
	MulticastRPCDestroyActor();
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
	// Player 예외처리 & 리턴
	AUNPlayerCharacter* Player = Cast<AUNPlayerCharacter>(Taker);
	if (!Player)
	{
		UE_LOG(LogTemp, Log, TEXT("Player is null"));
		return;
	}

	// 액터가 이미 삭제될 예정이라면 예외처리 & 리턴
	if (IsPendingKillPending())
	{
		UE_LOG(LogTemp, Log, TEXT("IsPendingKillPending is true"));
		return;
	}

	// ItemReference 예외처리 & 리턴
	if (!ItemReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somehow null!"));
		return;
	}
	
	// 인벤토리 컴포넌트 예외처리 & 리턴
	UUNInventoryComponent* PlayerInventory = Player->GetInventoryComponent();
	if (!PlayerInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null!"));
		return;
	}


	const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);
	switch (AddResult.OperationResult)
	{
		// 아이템이 추가가 안됐다면(Ex. 가방 용량 초과) 아무 변화 없음
	case EItemAddResult::IAR_NoItemAdded:
		break;

		// 일부만 먹었다면(Ex. 포션 5개 중 3개) 아이템 삭제 X. 개수 데이터만 업데이트
	case EItemAddResult::IAR_PartialAmountItemAdded:
		UpdateInteractableData();
		break;

		// 전부 먹었다면 필드 아이템은 삭제
	case EItemAddResult::IAR_AllItemAddeed:
		if (HasAuthority())
		{
			MulticastRPCDestroyActor();
		}
		else
		{
			Player->ServerRPCDestroyActor(this);
		}
		break;
	}

	// 결과 로그 출력
	UE_LOG(LogTemp, Log, TEXT("%s"), *AddResult.ResultMessage.ToString());

}

void AUNPickupObject::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapList.Add(OtherActor);

	if (bIsSelected && OtherActor == InteractingActor)
	{
		TakePickUp(OtherActor);
	}
}

void AUNPickupObject::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapList.Remove(OtherActor);

	if (OtherActor == InteractingActor)
	{
		EndInteract();
	}
}

void AUNPickupObject::Interact(AActor* Player)
{
	bIsSelected = true;
	InteractingActor = Player;

	if (OverlapList.Contains(InteractingActor))
	{
		TakePickUp(InteractingActor);
	}
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

void AUNPickupObject::MoveToFloor()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, 500.0f);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 1.0f);

	if (bHit)
	{
		FVector NewLocation = HitResult.Location;
		SetActorLocation(NewLocation);

		//DrawDebugSphere(GetWorld(), NewLocation, 25.0f, 12, FColor::Red, false, 2.0f);
	}
}

void AUNPickupObject::MulticastRPCDestroyActor_Implementation()
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMesh->SetVisibility(false);
	Mesh->SetVisibility(false);

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, [&]()
			{
				Destroy(true);

				if (GetWorld()->GetTimerManager().IsTimerActive(DestroyTimerHandle))
				{
					GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
				}
			}, 1.f, false);
	}
}