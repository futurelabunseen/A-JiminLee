// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LobbyGameMode.h"
#include "Player/UNPlayerController.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			//FTimerHandle timerhandle;
			//World->GetTimerManager().SetTimer(timerhandle, [&]() {
			//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "1");

			//	bUseSeamlessTravel = true;
			//	World->ServerTravel(FString("/Game/Maps/TestingMap?listen"));
			//	}, 5.f, false);

			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				AUNPlayerController* PlayerController = Cast<AUNPlayerController>(*It);
				if (PlayerController)
				{
					FInputModeUIOnly InputMode;
					PlayerController->SetInputMode(InputMode);

					PlayerController->MulticastRPCGameEndFunction();
				}
			}

			World->ServerTravel(FString("/Game/FantasyBundle/Maps/CustomCastle?listen"));
		}
	}
}
