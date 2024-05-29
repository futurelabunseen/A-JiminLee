// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UNGameMode.generated.h"

/**
 * 
 */

namespace MatchState
{
	extern PROEJCTUN_API const FName CountDown;
	extern PROEJCTUN_API const FName Farming;
	extern PROEJCTUN_API const FName Battle;
}

UCLASS()
class PROEJCTUN_API AUNGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AUNGameMode();

	//virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void OnMatchStateSet() override;

	UPROPERTY();
	float CachingTime;

	UPROPERTY()
	FTimerHandle GameStartTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	float WarmUpTime = 10.f;

	float LevelStartingTime = 0.f;

private:
	float CountdownTime = 0.f;

	//UFUNCTION()
	//void CanMove();
};
