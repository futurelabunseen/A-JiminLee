// Fill out your copyright notice in the Description page of Project Settings.


#include "UNGameState.h"
#include "Net/UnrealNetwork.h"

AUNGameState::AUNGameState()
{
	RemainingTime = MatchPlayTime;
}

// ��ũ�θ� ���� ��Ʈ��ũ ȯ�濡�� �������� ������ ����ȭ ���
void AUNGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUNGameState, RemainingTime);
}