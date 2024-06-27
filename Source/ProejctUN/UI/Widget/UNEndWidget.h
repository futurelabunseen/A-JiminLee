// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UNGASUserWidget.h"
#include "UNEndWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UButton;
class UMultiplayerSessionsSubsystem;
class APlayerController;

UCLASS()
class PROEJCTUN_API UUNEndWidget : public UUNGASUserWidget
{
	GENERATED_BODY()
	
public:
	void MenuSetup(bool bisDead);
	void MenuTearDown();

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EndPanelText;

	UFUNCTION()
	void ReturnButtonClicked();

	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	FTimerHandle GameEndTimerHandle;
};
