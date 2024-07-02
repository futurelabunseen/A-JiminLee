// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UNGASUserWidget.h"
#include "UUNLobbyWidget.generated.h"

/**
 * 
 */

class UButton;
UCLASS()
class PROEJCTUN_API UUUNLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void LobbyFunction();

	virtual void NativeConstruct() override;
};
