// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UNGASUserWidget.h"
#include "UNOverlayWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNOverlayWidget : public UUNGASUserWidget
{
	GENERATED_BODY()
	
public:
	UUNOverlayWidget();

	UFUNCTION()
	void SetCountDownText(FString NewText);

	UFUNCTION()
	void SetGameTimeText(FString NewText);

	UFUNCTION()
	void SetCountDownTextVisibility(bool option);

	UFUNCTION()
	void SetGameTimeTextVisibility(bool option);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Icon")
	UImage* QSkill;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Icon")
	UImage* WSkill;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Icon")
	UImage* ESkill;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Icon")
	UImage* RSkill;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Icon")
	UTextBlock* CountDownText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Icon")
	UTextBlock* GameTimeText;

};
