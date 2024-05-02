// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UNGASUserWidget.h"
#include "UNMainMenuWidget.generated.h"

class AUNPlayerCharacter;
/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNMainMenuWidget : public UUNGASUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AUNPlayerCharacter* PlayerCharacter;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
