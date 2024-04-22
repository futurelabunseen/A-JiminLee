// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UNInteractableObjectBase.generated.h"

class UBoxComponent;
class AUNPlayerCharacter;

UCLASS()
class PROEJCTUN_API AUNInteractableObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AUNInteractableObjectBase();

	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

	virtual void NotifyActorEndOverlap(class AActor* Other) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollision;

	// Mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION()
	void OpenItemPanel();

	UFUNCTION()
	void CloseItemPanel();
	
	UPROPERTY()
	TObjectPtr<AUNPlayerCharacter> PlayerCharacter;

	UPROPERTY()
	uint8 bisSelected;

	UPROPERTY()
	uint8 bisOverlap;
};
