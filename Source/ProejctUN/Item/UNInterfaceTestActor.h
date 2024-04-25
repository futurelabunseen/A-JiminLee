// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/UNInteractionInterface.h"
#include "UNInterfaceTestActor.generated.h"

UCLASS()
class PROEJCTUN_API AUNInterfaceTestActor : public AActor, public IUNInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AUNInterfaceTestActor();

protected:
	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

public:

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(AActor* Actor) override;
};
