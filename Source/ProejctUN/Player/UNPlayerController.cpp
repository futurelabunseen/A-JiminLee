// Fill out your copyright notice in the Description page of Project Settings.


#include "UNPlayerController.h"
#include "UNGASPlayerState.h"

#include "ProejctUN.h"

AUNPlayerController::AUNPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

// NetInit�� ����
void AUNPlayerController::PostNetInit()
{
	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();

	if (NetDriver)
	{
		if (NetDriver->ServerConnection)
		{
			UN_LOG(LogUNNetwork, Log, TEXT("Server Connection: %s"), *NetDriver->ServerConnection->GetName());
		}
	}
	else
	{
		UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
}

// ��Ʈ�ѷ��� Possess�ɶ� �Լ� ����
void AUNPlayerController::OnPossess(APawn* InPawn)
{
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::OnPossess(InPawn);
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
}
