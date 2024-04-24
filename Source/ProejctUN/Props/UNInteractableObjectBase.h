// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/UNInteractionInterface.h"
#include "UNInteractableObjectBase.generated.h"

UCLASS()
class PROEJCTUN_API AUNInteractableObjectBase : public AActor, public IUNInteractionInterface
{
	GENERATED_BODY()
	
public:
	AUNInteractableObjectBase();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void SendDataToController(UPrimitiveComponent* PC);

public:
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact() override;

	virtual void BeginPlay() override;


};
