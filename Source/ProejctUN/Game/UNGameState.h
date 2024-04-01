// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "UNGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AUNGameState();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(Transient, Replicated)
	int32 RemainingTime;

	int32 MatchPlayTime = 2000;
};
