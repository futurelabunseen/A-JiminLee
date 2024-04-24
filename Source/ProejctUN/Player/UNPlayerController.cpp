// Fill out your copyright notice in the Description page of Project Settings.


#include "UNPlayerController.h"
#include "UNGASPlayerState.h"

#include "ProejctUN.h"

AUNPlayerController::AUNPlayerController()
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bEnableTouchEvents = false;
}

void AUNPlayerController::CheckCursorOverObject(AActor* CursorOverObject)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *CursorOverObject->GetName());
	BeginOverInteractable(CursorOverObject);
}

void AUNPlayerController::ClearCursorOverObject(AActor* CursorOverObject)
{
	EndOverInteractable();
}


// NetInit후 실행
void AUNPlayerController::PostNetInit()
{
	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();

	if (NetDriver)
	{
		if (NetDriver->ServerConnection)
		{
			UN_LOG(LogUNNetwork, Log, TEXT("Server Connection: %s"), *NetDriver->ServerConnection->GetName());
		}
	}
	else
	{
		UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
}

// 컨트롤러가 Possess될때 함수 실행
void AUNPlayerController::OnPossess(APawn* InPawn)
{
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::OnPossess(InPawn);
	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
}





// Interact

void AUNPlayerController::BeginOverInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	TargetInteractable->BeginFocus();
}

// To Do .. : 변화주기
void AUNPlayerController::EndOverInteractable()
{
	if(IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AUNPlayerController::BeginInteract()
{
	// 재확인 함수

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, 
					&AUNPlayerController::Interact, TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void AUNPlayerController::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AUNPlayerController::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact();
	}
}