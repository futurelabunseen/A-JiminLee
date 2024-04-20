// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UNHUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UUNOverlayWidgetController;
class UUNGASInventoryWidgetController;
class UUNProgressBarWidgetController;
class UUNGASUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TObjectPtr<UUNGASUserWidget> OverlayWidget;
	
	UUNOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	

	UPROPERTY()
	TObjectPtr<UUNGASUserWidget> InventoryWidget;

	UUNGASInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);

	void InitInventory(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);


	UPROPERTY()
	TObjectPtr<UUNGASUserWidget> ProgressBarWidget;
	
	UUNProgressBarWidgetController* GetProgressBarWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitProgressBar(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ProgressBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UUNProgressBarWidgetController> ProgressBarWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNProgressBarWidgetController> ProgressBarWidgetControllerClass;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UUNGASInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNGASInventoryWidgetController> InventoryWidgetControllerClass;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UUNOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNOverlayWidgetController> OverlayWidgetControllerClass;
};
