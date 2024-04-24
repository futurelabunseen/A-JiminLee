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
	
protected:



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollision;

public:
	AUNInteractableBox();

	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

	virtual void NotifyActorEndOverlap(class AActor* Other) override;

	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;

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
