// Fill out your copyright notice in the Description page of Project Settings.


#include "UUNLobbyWidget.h"
#include "Components/Button.h"
#include "Game/LobbyGameMode.h"

bool UUUNLobbyWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void UUUNLobbyWidget::LobbyFunction()
{
	ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM) return;

	GM->MoveMap();
}

void UUUNLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (StartButton)
		StartButton->OnClicked.AddDynamic(this, &UUUNLobbyWidget::LobbyFunction);
}