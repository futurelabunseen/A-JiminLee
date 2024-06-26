// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UNHUD.h"
#include "UI/Widget/UNGASUserWidget.h"
#include "UI/WC/UNOverlayWidgetController.h"
#include "UI/WC/UNProgressBarWidgetController.h"
#include "UI/WC/UNInventoryWidgetController.h"
#include "UI/WC/UNBoxInventoryWidgetController.h"
#include "UI/Widget/UNInteractionWidget.h"
#include "UI/Widget/UNPlayerInventoryWidget.h"
#include "UI/Widget/UNOverlayWidget.h"
#include "UI/Widget/UNEndWidget.h"


#include "EngineUtils.h"

void AUNHUD::BeginPlay()
{
	Super::BeginPlay();
	//DefaultInitOverlay();
	InitBoxInventory();
	InitInteractionWidget();
	InitEndWidget();
}

UUNOverlayWidgetController* AUNHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UUNOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		//OverlayWidgetController->BindCallbacksToDependencies();

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AUNHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_UNHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_UNHUD"));

	OverlayWidget = CreateWidget<UUNOverlayWidget>(GetWorld(), OverlayWidgetClass);
	//OverlayWidget = Cast<UUNGASUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UUNOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	//WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}



UUNInventoryWidgetController* AUNHUD::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UUNInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);

		return InventoryWidgetController;
	}
	return InventoryWidgetController;
}

void AUNHUD::InitInventory(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(InventoryWidgetClass, TEXT("Inventory Widget Class uninitialized, please fill out BP_UNHUD"));
	checkf(InventoryWidgetControllerClass, TEXT("Inventory Widget Controller Class uninitialized, please fill out BP_UNHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
	InventoryWidget = Cast<UUNGASUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UUNInventoryWidgetController* WidgetController = GetInventoryWidgetController(WidgetControllerParams);

	InventoryWidget->SetWidgetController(WidgetController);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}


UUNBoxInventoryWidgetController* AUNHUD::GetBoxInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if (BoxInventoryWidgetController == nullptr)
	{
		BoxInventoryWidgetController = NewObject<UUNBoxInventoryWidgetController>(this, BoxInventoryWidgetControllerClass);
		BoxInventoryWidgetController->SetWidgetControllerParams(WCParams);

		return BoxInventoryWidgetController;
	}
	return BoxInventoryWidgetController;
}

void AUNHUD::InitBoxInventory()
{
	checkf(BoxInventoryWidgetClass, TEXT("Inventory Widget Class uninitialized, please fill out BP_UNHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), BoxInventoryWidgetClass);
	BoxInventoryWidget = Cast<UUNGASUserWidget>(Widget);

	BoxInventoryWidget->AddToViewport(3);
	BoxInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}


void AUNHUD::InitProgressBar(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(ProgressBarWidgetClass, TEXT("ProgressBarWidget Class uninitialized, please fill out BP_UNHUD"));
	checkf(ProgressBarWidgetControllerClass, TEXT("ProgressBarWidgetController Class uninitialized, please fill out BP_UNHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), ProgressBarWidgetClass);
	ProgressBarWidget = Cast<UUNGASUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UUNProgressBarWidgetController* WidgetController = GetProgressBarWidgetController(WidgetControllerParams);

	ProgressBarWidget->SetWidgetController(WidgetController);
}

UUNProgressBarWidgetController* AUNHUD::GetProgressBarWidgetController(const FWidgetControllerParams& WCParams)
{
	if (ProgressBarWidgetController == nullptr)
	{
		ProgressBarWidgetController = NewObject<UUNProgressBarWidgetController>(this, ProgressBarWidgetControllerClass);
		ProgressBarWidgetController->SetWidgetControllerParams(WCParams);

		return ProgressBarWidgetController;
	}
	return ProgressBarWidgetController;
}

void AUNHUD::InitEndWidget()
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), EndWidgetClass);
	EndWidget = Cast<UUNEndWidget>(Widget);
}

void AUNHUD::InitInteractionWidget()
{
	checkf(InteractionWidgetClass, TEXT("InteractionWidget Class uninitialized, please fill out BP_UNHUD"));

	InteractionWidget = CreateWidget<UUNInteractionWidget>(GetWorld(), InteractionWidgetClass);
	InteractionWidget->AddToViewport(-1);
	InteractionWidget->SetIsEnabled(false);
	InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
}

// TESTING
void AUNHUD::OpenInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	bisInventoryOpen = true;
}

void AUNHUD::CloseInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	bisInventoryOpen = false;
}

void AUNHUD::OpenEndWidget()
{
	if (EndWidget)
	{
		EndWidget->MenuSetup();
	}
}

void AUNHUD::CloseEndWidget()
{
	if (EndWidget)
	{
		EndWidget->MenuTearDown();
	}
}

void AUNHUD::ShowInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AUNHUD::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AUNHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void AUNHUD::ToggleInventory()
{
	if (bisInventoryOpen)
	{
		CloseInventory();
	}
	else
	{
		OpenInventory();
	}
}

void AUNHUD::OpenBoxInventory()
{
	BoxInventoryWidget->SetVisibility(ESlateVisibility::Visible);
}

void AUNHUD::CloseBoxInventory()
{
	BoxInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AUNHUD::SetCountDownTextVisibility(bool option)
{
	if (OverlayWidget)
	{
		OverlayWidget->SetCountDownTextVisibility(option);
	}
}

void AUNHUD::SetCountDownMsgVisibility(bool option)
{
	if (OverlayWidget)
	{
		OverlayWidget->SetCountDownMsgVisibility(option);
	}
}

void AUNHUD::SetCountDownText(FString Text)
{
	if (OverlayWidget)
	{
		OverlayWidget->SetCountDownText(Text);
	}
}

void AUNHUD::SetCountDownMsg(FString Text)
{
	if (OverlayWidget)
	{
		OverlayWidget->SetCountDownMsg(Text);
	}
}

void AUNHUD::SetGameTimeTextVisibility(bool option)
{
	if (OverlayWidget)
	{
		OverlayWidget->SetGameTimeTextVisibility(option);
	}
}

void AUNHUD::SetGameTimeText(FString Text)
{
	if (OverlayWidget)
	{
		OverlayWidget->SetGameTimeText(Text);
	}
}

void AUNHUD::DefaultInitOverlay()
{
	OverlayWidget = CreateWidget<UUNOverlayWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->AddToViewport();
}


