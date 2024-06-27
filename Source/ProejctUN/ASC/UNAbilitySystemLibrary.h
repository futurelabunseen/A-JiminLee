// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UNAbilitySystemLibrary.generated.h"

class UUNOverlayWidgetController;
class UUNInventoryWidgetController;

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "UNAbilitySystemLibrary|WidgetController")
	static UUNOverlayWidgetController* GetoverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "UNAbilitySystemLibrary|WidgetController")
	static UUNInventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);
};
