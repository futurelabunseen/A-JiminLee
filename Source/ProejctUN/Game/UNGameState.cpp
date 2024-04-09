// Fill out your copyright notice in the Description page of Project Settings.


#include "UNGameState.h"
#include "Net/UnrealNetwork.h"

AUNGameState::AUNGameState()
{
	RemainingTime = MatchPlayTime;
}

// 매크로를 통해 네트워크 환경에서 데이터의 복제와 동기화 담당
void AUNGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUNGameState, RemainingTime);
}