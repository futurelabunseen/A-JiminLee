// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/UNInteractableObjectBase.h"
#include "UNInteractableBox.generated.h"

class UBoxComponent;
class AUNPlayerCharacter;

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNInteractableBox : public AUNInteractableObjectBase
{
	GENERATED_BODY()
	

public:
	AUNInteractableBox();

	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

	virtual void NotifyActorEndOverlap(class AActor* Other) override;

	UFUNCTION(BlueprintCallable)
	virtual void Interact(AActor* Actor) override;

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
