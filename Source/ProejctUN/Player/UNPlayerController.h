// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/UNInteractionInterface.h"
#include "GameFramework/PlayerController.h"
#include "UNPlayerController.generated.h"

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
	virtual void PostNetInit() override;
	virtual void OnPossess(APawn* InPawn) override;
	// Interaction
public:
	UFUNCTION()
	void CheckCursorOverObject(AActor* CursorOverObject);

	UFUNCTION()
	void ClearCursorOverObject(AActor* CursorOverObject);

protected:

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }

	UPROPERTY(VisibleAnywhere, Category = "Controller | Interaction")
	TScriptInterface<IUNInteractionInterface> TargetInteractable;


	UPROPERTY(VisibleAnywhere, Category = "Controller | Interaction")
	TScriptInterface<IUNInteractionInterface> CurrentInteractingObject;


	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

public:
	void BeginOverInteractable(AActor* NewInteractable);
	void EndOverInteractable();
	void BeginInteract();
	void EndInteract();
	void Interact();
};
