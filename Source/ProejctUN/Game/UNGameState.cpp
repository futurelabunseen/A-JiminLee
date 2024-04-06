// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UNGameState.h"
#include "Net/UnrealNetwork.h"

AUNGameState::AUNGameState()
{
	RemainingTime = MatchPlayTime;
}

void AUNGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUNGameState, RemainingTime);
}