// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UNGASUserWidget.h"
#include "UNOverlayWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNOverlayWidget : public UUNGASUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage* QSkill;
};
