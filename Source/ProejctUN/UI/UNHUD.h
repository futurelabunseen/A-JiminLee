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
class UUNBoxInventoryWidgetController;
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
	UUNOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	

	//UPROPERTY()
	//TObjectPtr<UUNGASUserWidget> BoxInventoryWidget;

	UUNBoxInventoryWidgetController* GetBoxInventoryWidgetController(const FWidgetControllerParams& WCParams);

	//void InitInventory(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);


	UUNGASInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);

	void InitInventory(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	
	UUNProgressBarWidgetController* GetProgressBarWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitProgressBar(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	int8 bisInventoryOpen;

private:
	UPROPERTY()
	TObjectPtr<UUNGASUserWidget> ProgressBarWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ProgressBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UUNProgressBarWidgetController> ProgressBarWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNProgressBarWidgetController> ProgressBarWidgetControllerClass;

	
	
	UPROPERTY()
	TObjectPtr<UUNGASUserWidget> InventoryWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UUNGASInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNGASInventoryWidgetController> InventoryWidgetControllerClass;



	UPROPERTY()
	TObjectPtr<UUNGASUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UUNOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNOverlayWidgetController> OverlayWidgetControllerClass;


	UPROPERTY()
	TObjectPtr<UUNBoxInventoryWidgetController> BoxInventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNBoxInventoryWidgetController> BoxInventoryWidgetControllerClass;

public:
	UFUNCTION()
	void OpenInventory();

	UFUNCTION()
	void CloseInventory();

	UFUNCTION()
	void OpenBoxInventory();

	UFUNCTION()
	void CloseBoxInventory();
};
