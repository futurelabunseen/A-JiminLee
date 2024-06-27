// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WC/UNWidgetController.h"
#include "UNInventoryWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROEJCTUN_API UUNInventoryWidgetController : public UUNWidgetController
{
	GENERATED_BODY()
	
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
