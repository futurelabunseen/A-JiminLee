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

// ReturnToMainMenu버튼을 눌렀을 때
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

	// 클라
	if (!PC->HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("ClientPlayer!"));
		
		// ServerRPC->MulticastRPC를 통해 모든 타이머를 종료 (ServerRPCGameEndFunction = 타이머 종료 함수)
		PC->ServerRPCGameEndFunction();
		GetWorld()->GetTimerManager().SetTimer(GameEndTimerHandle, [PC]()
			{
				// 클라 세션 나가기 (3초 딜레이는 타이머 종료 시간이 필요하기 때문)
				UE_LOG(LogTemp, Log, TEXT("ActivateTimer!"));
				PC->ClientReturnToMainMenuWithTextReason(FText()); 
			}, 3.f, false);

		return;
	}
	
	// 서버
	UE_LOG(LogTemp, Log, TEXT("ServerPlayer!"));
	AUNGameMode* GM = Cast<AUNGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		// 자기 자신이 아닌 모든 플레이어의 Timer종료함수
		GM->GameEndClearHandle();

		// 1초뒤에 모든 PlayerController순회하며 ClientReturnToMainMenuWithTextReason발동
		FTimerHandle AllPlayerKickTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(AllPlayerKickTimerHandle, [GM]()
			{
				UE_LOG(LogTemp, Log, TEXT("ActivateKickTimer!"));
				GM->AllPlayerKick();
			}, 1.f, false);


		// 세션 지우기
		// To Do .. : if문으로 PlayerController가 서버만 남았는지 확인
		FTimerHandle DestroySessionTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroySessionTimerHandle, [this]()
			{
				UE_LOG(LogTemp, Log, TEXT("ActivateDestroyTimer!"));
				MultiplayerSessionsSubsystem->DestroySession();
			}, 3.f, false);
	}
}
