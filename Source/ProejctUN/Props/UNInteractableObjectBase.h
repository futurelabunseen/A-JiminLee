// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/UNInteractionInterface.h"
#include "UNInteractableObjectBase.generated.h"

class UBoxComponent;
UCLASS()
class PROEJCTUN_API AUNInteractableObjectBase : public AActor, public IUNInteractionInterface
{
	GENERATED_BODY()
	
public:
	AUNInteractableObjectBase();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Test Actor")
	USkeletalMeshComponent* SkeletalMesh;

	UFUNCTION()
	void SendBeginDataToController(UPrimitiveComponent* OverlapActor);

	UFUNCTION()
	void SendEndDataToController(UPrimitiveComponent* OverlapActor);

	UFUNCTION()
	void SendBeginDataToController2(UPrimitiveComponent* OverlapActor);

	UFUNCTION()
	void SendEndDataToController2(UPrimitiveComponent* OverlapActor);

public:
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(AActor* Actor) override;


};
