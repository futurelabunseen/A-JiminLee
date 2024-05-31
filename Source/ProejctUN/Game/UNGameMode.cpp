// Fill out your copyright notice in the Description page of Project Settings.


#include "UNGameMode.h"
#include "ProejctUN.h"
#include "UNGameState.h"
#include "Player/UNGASPlayerState.h"
#include "Player/UNPlayerController.h"
#include "Props/UNPickupObject.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

namespace MatchState
{
	const FName CountDown = FName("CountDown");
	const FName Farming = FName("Farming");
	const FName Battle = FName("Battle");
}

AUNGameMode::AUNGameMode() :
	MinSpawnCount(50),
	MaxSpawnCount(100),
	//MinSpawnLocation(-1000.f, -1000.f, 10.f),
	//MaxSpawnLocation(1000.f, 1000.f, 10.f) 
	MinSpawnLocation(-48000.f, 13500.f, -22600.f),
	MaxSpawnLocation(-44000.f, 16000.f, -22600.f)
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

void AUNGameMode::BeginPlay()
{	
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();

	SpawnProps();

	FTimerHandle CountDownTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, [&]()
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "GameStart");
			SetMatchState(MatchState::CountDown);
		}, 1.f, false);
}

//void AUNGameMode::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}


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
	
	FInputModeUIOnly InputMode;
	NewPlayer->SetInputMode(InputMode);

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

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			//StartMatch();
		}
	}

	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AUNGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AUNPlayerController* PlayerController = Cast<AUNPlayerController>(*It);
		if (PlayerController)
		{
			PlayerController->OnMatchStateSet(MatchState);
		}
	}
}
// ==================== 로그인 관련 ==================== End

void AUNGameMode::SpawnProps()
{
	if (UWorld* World = GetWorld())
	{
		int SpawnCnt = FMath::RandRange(MinSpawnCount, MaxSpawnCount - 1);

		for (int cnt = 0; cnt < SpawnCnt; cnt++)
		{
			// 테스트용 랜덤
			float RandomX = FMath::RandRange(MinSpawnLocation.X, MaxSpawnLocation.X);
			float RandomY = FMath::RandRange(MinSpawnLocation.Y, MaxSpawnLocation.Y);
			float RandomZ = FMath::RandRange(MinSpawnLocation.Z, MaxSpawnLocation.Z);
			
			FVector SpawnLoc = FVector(RandomX, RandomY, RandomZ);
			FActorSpawnParameters SpawnParams;
			AUNPickupObject* SpawnedActor = World->SpawnActor<AUNPickupObject>(AUNPickupObject::StaticClass(), SpawnLoc, FQuat::Identity.Rotator(), SpawnParams);
			SpawnedItems.Add(SpawnedActor);
		}
	}
}
