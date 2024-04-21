// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UNHUD.h"
#include "UI/Widget/UNGASUserWidget.h"
#include "UI/WC/UNOverlayWidgetController.h"
#include "UI/WC/UNProgressBarWidgetController.h"
#include "UI/WC/UNGASInventoryWidgetController.h"

#include "EngineUtils.h"

UUNOverlayWidgetController* AUNHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UUNOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AUNHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_UNHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_UNHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UUNGASUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UUNOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	Widget->AddToViewport();
}



UUNGASInventoryWidgetController* AUNHUD::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UUNGASInventoryWidgetController>(this, InventoryWidgetControllerClass);
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
	UUNGASInventoryWidgetController* WidgetController = GetInventoryWidgetController(WidgetControllerParams);

	InventoryWidget->SetWidgetController(WidgetController);
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