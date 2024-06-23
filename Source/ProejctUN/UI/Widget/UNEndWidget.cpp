// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNEndWidget.h"
#include "GameFramework/PlayerController.h"
#include "Player/UNPlayerController.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"

void UUNEndWidget::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (AUNPlayerController* PC = Cast<AUNPlayerController>(PlayerController))
		{
			PC->SetKeyBoardInputMode(false);
		}
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

	//if (ReturnButton)
	//{
	//	ReturnButton->OnClicked.AddDynamic(this, &UUNEndWidget::ReturnButtonClicked);
	//}

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
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode)
		{
			UE_LOG(LogTemp, Log, TEXT("ReturnToMainMenuHost!!!"));
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
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			PlayerController->SetInputMode(InputModeData);
		}
	}
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

	if (MultiplayerSessionsSubsystem)
	{
		if (AUNPlayerController* PC = Cast<AUNPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			PC->MulticastRPCGameEndFunction();

			//if(PC->HasAuthority())
			//{
			//	MultiplayerSessionsSubsystem->DestroySession();
			//}
			//else
			//{
			//	PC->ClientReturnToMainMenuWithTextReason(FText());
			//}
		}

		UE_LOG(LogTemp, Log, TEXT("TryDestroySession!!!"));
		
		MultiplayerSessionsSubsystem->DestroySession();
	}
}
