// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LobbyGameMode.h"
#include "Player/UNPlayerController.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AUNPlayerController* PC = Cast<AUNPlayerController>(NewPlayer))
	{
		PC->SetKeyBoardInputMode(true);
	}
}

void ALobbyGameMode::CharacterHandleClear()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AUNPlayerController* PlayerController = Cast<AUNPlayerController>(*It);
		if (PlayerController)
		{
			PlayerController->MulticastRPCGameEndFunction();
		}
	}
}

void ALobbyGameMode::MoveToCastle()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/FantasyBundle/Maps/CustomCastle?listen"));
	}
}

void ALobbyGameMode::MoveMap()
{
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FTimerHandle CharacterHandleCleartimerhandle;
			World->GetTimerManager().SetTimer(CharacterHandleCleartimerhandle, this, &ALobbyGameMode::CharacterHandleClear, 2.f, false);

			FTimerHandle timerhandle;
			World->GetTimerManager().SetTimer(timerhandle, this, &ALobbyGameMode::MoveToCastle, 6.f, false);

		}
	}
}
