// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/UNInteractionInterface.h"
#include "GameFramework/PlayerController.h"
#include "UNPlayerController.generated.h"

class AUNHUD;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};
/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AUNPlayerController();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PostNetInit() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Interaction
public:
	UFUNCTION()
	void CheckCursorOverObject(AActor* CursorOverObject);

	UFUNCTION()
	void ClearCursorOverObject(AActor* CursorOverObject);

	void OnMatchStateSet(FName State);
protected:

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }

	UPROPERTY(VisibleAnywhere, Category = "Controller | Interaction")
	TScriptInterface<IUNInteractionInterface> TargetInteractable;


	UPROPERTY(VisibleAnywhere, Category = "Controller | Interaction")
	TScriptInterface<IUNInteractionInterface> CurrentInteractingObject;


	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState, VisibleAnywhere)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	AUNHUD* HUD;

	UPROPERTY()
	int CountDownValue;

	UPROPERTY()
	int GameTimeValue;
public:
	UFUNCTION()
	void CountDownFunction(int Value);

	UFUNCTION()
	void FarmingFunction(int Value);

	UFUNCTION()
	void BattleFunction(int Value);

	void BeginOverInteractable(AActor* NewInteractable);
	void EndOverInteractable();
	void BeginInteract();
	void EndInteract();
	void Interact();

	FTimerHandle CountDownTimerHandle;
	FTimerHandle GameTimeTimerHandle;

	UFUNCTION(Server, Unreliable)
	void ServerRPCRequestCurrentTime();

	UFUNCTION(Client, Unreliable)
	void ClientRPCRequestCurrentTime(FName ServerMatchState, int ServerTime);

	UFUNCTION(Client, Unreliable)
	void ClientRPCOpenEndWidget();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCGameEndFunction();

	UFUNCTION()
	void SetKeyBoardInputMode(bool bKeyboard);

	void SetCharacterMovementMode(bool bMovement);

	int8 bisFarmingDone;
};
