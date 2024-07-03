// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Struct/DecalStruct.h"
#include "DecalSystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UDecalSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROEJCTUN_API IDecalSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UDecalComponent* GetDecalComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FDecalStruct GetCurrentActiveDacalData();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCurrentActiveDacalData(FDecalStruct NewDecalData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ClearCurrentActiveDacalData();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ActivateDecal(FDecalStruct DecalStruct);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndDecal();
};
