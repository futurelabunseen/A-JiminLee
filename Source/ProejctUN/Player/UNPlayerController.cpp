// Fill out your copyright notice in the Description page of Project Settings.


#include "UNPlayerController.h"
#include "UNGASPlayerState.h"
#include "UI/UNHUD.h"
#include "Net/UnrealNetwork.h"
#include "Game/UNGameMode.h"
#include "Character/UNPlayerCharacter.h"

#include "ProejctUN.h"

AUNPlayerController::AUNPlayerController()
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bEnableTouchEvents = false;
	bReplicates = true;
}

void AUNPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AUNPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//if (IsLocalController())
	//{
	//	FTimerHandle CheckingTimeTimerHandle;
	//	GetWorld()->GetTimerManager().SetTimer(CheckingTimeTimerHandle,this, &AUNPlayerController::ServerRPCRequestCurrentTime, 3.f, true, 1.f);
	//}
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

void AUNPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUNPlayerController, MatchState);
}

// Interact
void AUNPlayerController::CheckCursorOverObject(AActor* CursorOverObject)
{
	BeginOverInteractable(CursorOverObject);
}

void AUNPlayerController::ClearCursorOverObject(AActor* CursorOverObject)
{
	EndOverInteractable();
}

void AUNPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, "InProgress!");
	}
	else if (MatchState == MatchState::CountDown)
	{
		CountDownFunction(5);
	}
	else if (MatchState == MatchState::Farming)
	{
		FarmingFunction(20);
	}
	else if (MatchState == MatchState::Battle)
	{
		BattleFunction(100);
	}
}

void AUNPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, "InProgress!");
	}
	else if (MatchState == MatchState::CountDown)
	{
		CountDownFunction(5);
	}
	else if (MatchState == MatchState::Farming)
	{
		FarmingFunction(20);
	}
	else if (MatchState == MatchState::Battle)
	{
		BattleFunction(100);
	}
}

void AUNPlayerController::CountDownFunction(int Value)
{
	StopMovement();
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(GetCharacter());
	if (PlayerCharacter)
	{
		PlayerCharacter->StopMovement();
	}
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	HUD = Cast<AUNHUD>(GetHUD());
	if (HUD)
	{
		HUD->SetGameTimeTextVisibility(false);
		HUD->SetCountDownTextVisibility(true);
	}

	CountDownValue = Value;
	GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, [&]() {
		if(HUD)
		{
			HUD->SetCountDownText(FString::FromInt(CountDownValue));
		}
		CountDownValue -= 1;
		if (CountDownValue < 0)
		{
			if (bisFarmingDone)
			{
				OnMatchStateSet(MatchState::Battle);
			}
			else
			{
				OnMatchStateSet(MatchState::Farming);
			}
			GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);
		}
		}, 1.f, true, 0.f);
}

void AUNPlayerController::FarmingFunction(int Value)
{
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(GetCharacter());
	if (PlayerCharacter)
	{
		PlayerCharacter->ActivateMovement();
	}
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);
	HUD = Cast<AUNHUD>(GetHUD());
	if (HUD)
	{
		HUD->SetCountDownTextVisibility(false);
		HUD->SetGameTimeTextVisibility(true);
	}

	GameTimeValue = Value;
	GetWorld()->GetTimerManager().SetTimer(GameTimeTimerHandle, [&]() {
		if(HUD)
		{
			HUD->SetGameTimeText(FString::FromInt(GameTimeValue));
		}
		GameTimeValue -= 1;
		if (GameTimeValue < 0)
		{
			bisFarmingDone = true;
			OnMatchStateSet(MatchState::CountDown);

			GetWorld()->GetTimerManager().ClearTimer(GameTimeTimerHandle);
		}
		}, 1.f, true, 0.f);
}

void AUNPlayerController::BattleFunction(int Value)
{
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(GetCharacter());
	if (PlayerCharacter)
	{
		PlayerCharacter->ActivateMovement();
	}
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);
	HUD = Cast<AUNHUD>(GetHUD());
	if (HUD)
	{
		HUD->SetCountDownTextVisibility(false);
		HUD->SetGameTimeTextVisibility(true);
	}
	GameTimeValue = Value;
	GetWorld()->GetTimerManager().SetTimer(GameTimeTimerHandle, [&]() {
		if (HUD)
		{
			HUD->SetGameTimeText(FString::FromInt(GameTimeValue));
		}
		GameTimeValue -= 1;
		if (GameTimeValue < 0)
		{
			bisFarmingDone = true;
			OnMatchStateSet(MatchState::CountDown);

			GetWorld()->GetTimerManager().ClearTimer(GameTimeTimerHandle);
		}
		}, 1.f, true, 0.f);
}

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

	//if (AUNHUD* HUD = Cast<AUNHUD>(GetHUD()))
	//{
	//	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	//}

	TargetInteractable->BeginFocus();
	
}

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

		//if (AUNHUD* HUD = Cast<AUNHUD>(GetHUD()))
		//{
		//	HUD->HideInteractionWidget();
		//}

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AUNPlayerController::BeginInteract()
{
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();
			CurrentInteractingObject = TargetInteractable;

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
	else
	{
		if (CurrentInteractingObject && CurrentInteractingObject != TargetInteractable)
		{
			CurrentInteractingObject->EndInteract();
			CurrentInteractingObject = nullptr;
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
		TargetInteractable->Interact(GetPawn());
	}
}

void AUNPlayerController::ServerRPCRequestCurrentTime_Implementation()
{
	if (MatchState == MatchState::InProgress)
	{
		//ClientRPCRequestCurrentTime(MatchState, GameTimeValue);
	}
	else if (MatchState == MatchState::CountDown)
	{
		ClientRPCRequestCurrentTime(MatchState, CountDownValue);
	}
	else if (MatchState == MatchState::Farming)
	{
		ClientRPCRequestCurrentTime(MatchState, GameTimeValue);
	}
	else if (MatchState == MatchState::Battle)
	{
		ClientRPCRequestCurrentTime(MatchState, GameTimeValue);
	}
}

void AUNPlayerController::ClientRPCRequestCurrentTime_Implementation(FName ServerMatchState, int ServerTime)
{
	if (ServerMatchState == MatchState::InProgress)
	{

	}
	else if (ServerMatchState == MatchState::CountDown)
	{
		CountDownValue = ServerTime;
	}
	else if (MatchState == MatchState::Farming)
	{
		GameTimeValue = ServerTime;
	}
	else if (MatchState == MatchState::Battle)
	{
		GameTimeValue = ServerTime;
	}
}