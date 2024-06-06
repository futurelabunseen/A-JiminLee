// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UNGameMode.generated.h"

/**
 * 
 */

class AUNPickupObject;
class AUNPlayerCharacter;

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
	virtual void Logout(AController* Exiting) override;
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


public:
	UPROPERTY(EditAnywhere)
	int MinSpawnCount;

	UPROPERTY(EditAnywhere)
	int MaxSpawnCount;

	UPROPERTY(EditAnywhere)
	FVector MinSpawnLocation;

	UPROPERTY(EditAnywhere)
	FVector MaxSpawnLocation;

	UFUNCTION()
	void SpawnProps();

	UFUNCTION()
	void OnCharacterDeath(AUNCharacter* Character);

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AUNPickupObject> > SpawnedItems;

	TArray<AUNCharacter*> PlayerArray;

};
