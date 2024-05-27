// Fill out your copyright notice in the Description page of Project Settings.


#include "UNGameMode.h"
#include "ProejctUN.h"
#include "UNGameState.h"
#include "Player/UNGASPlayerState.h"
#include "Player/UNPlayerController.h"

AUNGameMode::AUNGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/CoreUObject.Class'/Script/ProejctUN.UNCharacter'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/ProejctUN.UNPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	GameStateClass = AUNGameState::StaticClass();
	PlayerStateClass = AUNGASPlayerState::StaticClass();
}

// ==================== 로그인 관련 ==================== Start

void AUNGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
}

APlayerController* AUNGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("Begin"));
	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
	return NewPlayerController;
}

void AUNGameMode::PostLogin(APlayerController* NewPlayer)
{
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		if (NetDriver->ClientConnections.Num() == 0)
		{
			UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("No Client Connection"));
		}
		else
		{
			for (const auto& Connection : NetDriver->ClientConnections)
			{
				UN_LOG(LogUNNetwork, Log, TEXT("Client Connection: %s"), *Connection->GetName());
			}
		}
	}

	//int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	//if (NumberOfPlayers == 2)
	//{
	//	UWorld* World = GetWorld();
	//	if (World)
	//	{
	//		bUseSeamlessTravel = true;
	//		World->ServerTravel(FString("/Game/Maps/TestingMap?listen"));
	//	}
	//}

	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
}

// ==================== 로그인 관련 ==================== End