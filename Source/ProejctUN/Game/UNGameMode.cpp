// Fill out your copyright notice in the Description page of Project Settings.


#include "UNGameMode.h"
#include "ProejctUN.h"
#include "../Player/UNPlayerController.h"

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
}

void AUNGameMode::BeginPlay()
{
	Super::BeginPlay();
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("Begin"));
}

//APlayerController* AUNGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
//{
//	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("Begin"));
//	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
//	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
//	return nullptr;
//}
