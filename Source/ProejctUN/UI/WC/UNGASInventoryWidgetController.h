// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WC/UNWidgetController.h"
#include "UNGASInventoryWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROEJCTUN_API UUNGASInventoryWidgetController : public UUNWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
