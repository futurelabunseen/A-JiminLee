// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UNGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	AUNGameMode();

	//virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void BeginPlay() override;
};
