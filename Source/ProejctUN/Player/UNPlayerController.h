// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UNPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AUNPlayerController();

protected:
	virtual void PostNetInit() override;
	virtual void OnPossess(APawn* InPawn) override;
};
