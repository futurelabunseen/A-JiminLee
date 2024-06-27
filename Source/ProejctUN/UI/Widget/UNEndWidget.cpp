// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNEndWidget.h"
#include "GameFramework/PlayerController.h"
#include "Player/UNPlayerController.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Game/UNGameMode.h"
#include "Components/TextBlock.h"

void UUNEndWidget::MenuSetup(bool bisDead)
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	if (bisDead)
	{
		EndPanelText->SetVisibility(ESlateVisibility::Visible);

		UWorld* World = GetWorld();
		if (World)
		{
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if (AUNPlayerController* PC = Cast<AUNPlayerController>(PlayerController))
			{
				PC->SetKeyBoardInputMode(false);
			}
		}
	}
	else
	{
		EndPanelText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ReturnButton && !ReturnButton->OnClicked.IsBound())
	{
		ReturnButton->OnClicked.AddDynamic(this, &UUNEndWidget::ReturnButtonClicked);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UUNEndWidget::OnDestroySession);
		}
	}
}

bool UUNEndWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void UUNEndWidget::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ReturnButton->SetIsEnabled(true);
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		//AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		AUNGameMode* GameMode = World->GetAuthGameMode<AUNGameMode>();
		if (GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UUNEndWidget::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	//if (World)
	//{
	//	PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
	//	if (PlayerController)
	//	{
	//		FInputModeGameAndUI InputModeData;
	//		PlayerController->SetInputMode(InputModeData);
	//	}
	//}
	if (ReturnButton && !ReturnButton->OnClicked.IsBound())
	{
		ReturnButton->OnClicked.RemoveDynamic(this, &UUNEndWidget::ReturnButtonClicked);
	}
	if (MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &UUNEndWidget::OnDestroySession);
	}
}

void UUNEndWidget::ReturnButtonClicked()
{
	ReturnButton->SetIsEnabled(false);

	if (!MultiplayerSessionsSubsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("MultiplayerSessionsSubsystem is null!"));
		return;
	}

	AUNPlayerController* PC = Cast<AUNPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PC)
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerController is null!"));
		return;
	}

	if (!PC->HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("ClientPlayer!"));
		
		PC->ServerRPCGameEndFunction();
		GetWorld()->GetTimerManager().SetTimer(GameEndTimerHandle, [PC]()
			{
				UE_LOG(LogTemp, Log, TEXT("ActivateTimer!"));
				PC->ClientReturnToMainMenuWithTextReason(FText());
			}, 3.f, false);

		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ServerPlayer!"));
	AUNGameMode* GM = Cast<AUNGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->GameEndClearHandle();

		FTimerHandle AllPlayerKickTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(AllPlayerKickTimerHandle, [GM]()
			{
				UE_LOG(LogTemp, Log, TEXT("ActivateKickTimer!"));
				GM->AllPlayerKick();
			}, 1.f, false);


		FTimerHandle DestroySessionTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroySessionTimerHandle, [this]()
			{
				UE_LOG(LogTemp, Log, TEXT("ActivateDestroyTimer!"));
				MultiplayerSessionsSubsystem->DestroySession();
			}, 3.f, false);
	}
}
