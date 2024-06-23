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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	virtual void Interact(AActor* Player) override;
	virtual void EndInteract() override;

	virtual void NotifyActorBeginOverlap(class AActor* Other) override;
	virtual void NotifyActorEndOverlap(class AActor* Other) override;

	void UpdateInteractableData();
	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);
	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);
	void TakePickUp(AActor* Taker);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(ReplicatedUsing = OnRep_DesiredItemID ,EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	UPROPERTY(VisibleAnywhere)
	bool bIsSelected;

	UPROPERTY(VisibleAnywhere)
	AActor* InteractingActor;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> OverlapList;

	FTimerHandle TimerHandle;

	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_DesiredItemID();

	UFUNCTION()
	void InitializeDropItem(FName ID, int32 Quantity);

	UFUNCTION(Server, Unreliable)
	void ServerRPCDestoryActor();

	UFUNCTION()
	void MoveToFloor();
//
//#if WITH_EDITOR
//	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) override;
//#endif
};
