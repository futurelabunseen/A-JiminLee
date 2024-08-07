// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UNHUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UUNOverlayWidgetController;
class UUNInventoryWidgetController;
class UUNProgressBarWidgetController;
class UUNBoxInventoryWidgetController;
class UUNOverlayWidget;
class UUNGASUserWidget;
class UUNEndWidget;
struct FWidgetControllerParams;

struct FInteractableData;
class UUNInteractionWidget;
class UUNPlayerInventoryWidget;
/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UUNOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);


	UUNBoxInventoryWidgetController* GetBoxInventoryWidgetController(const FWidgetControllerParams& WCParams);

	void InitBoxInventory();


	UUNInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);

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
	TObjectPtr<UUNInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNInventoryWidgetController> InventoryWidgetControllerClass;



	UPROPERTY()
	TObjectPtr<UUNOverlayWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UUNOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNOverlayWidgetController> OverlayWidgetControllerClass;


	UPROPERTY()
	TObjectPtr<UUNGASUserWidget> BoxInventoryWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BoxInventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UUNBoxInventoryWidgetController> BoxInventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUNBoxInventoryWidgetController> BoxInventoryWidgetControllerClass;

	//======================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY()
	TObjectPtr<UUNInteractionWidget> InteractionWidget;

	UPROPERTY()
	TObjectPtr<class UUNEndWidget> EndWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> EndWidgetClass;

	void InitEndWidget();

	void InitInteractionWidget();

public:
	UFUNCTION()
	void OpenInventory();

	UFUNCTION()
	void CloseInventory();

	UFUNCTION()
	void OpenEndWidget(bool bisDead);

	UFUNCTION()
	void CloseEndWidget();

	UFUNCTION()
	void ShowInteractionWidget();

	UFUNCTION()
	void HideInteractionWidget();

	void UpdateInteractionWidget(const FInteractableData* InteractableData);

	UFUNCTION()
	void ToggleInventory();

	UFUNCTION()
	void OpenBoxInventory();

	UFUNCTION()
	void CloseBoxInventory();

	UFUNCTION()
	void SetCountDownTextVisibility(bool option);

	UFUNCTION()
	void SetCountDownMsgVisibility(bool option);

	UFUNCTION()
	void SetCountDownText(FString Text);

	UFUNCTION()
	void SetCountDownMsg(FString Text);

	UFUNCTION()
	void SetGameTimeTextVisibility(bool option);

	UFUNCTION()
	void SetGameTimeText(FString Text);

	UFUNCTION()
	void DefaultInitOverlay();
};
