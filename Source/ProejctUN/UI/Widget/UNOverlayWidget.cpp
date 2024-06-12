// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UNOverlayWidget.h"
#include "Components/TextBlock.h"

UUNOverlayWidget::UUNOverlayWidget()
{
}

void UUNOverlayWidget::SetCountDownText(FString NewText)
{
	if (CountDownText)
	{
		CountDownText->SetText(FText::FromString(NewText));
	}
}

void UUNOverlayWidget::SetCountDownMsg(FString NewText)
{
	if (CountDownMsg)
	{
		CountDownMsg->SetText(FText::FromString(NewText));
	}
}

void UUNOverlayWidget::SetCountDownTextVisibility(bool option)
{
	if (option)
	{
		CountDownText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CountDownText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUNOverlayWidget::SetCountDownMsgVisibility(bool option)
{
	if (option)
	{
		CountDownMsg->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CountDownMsg->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUNOverlayWidget::SetGameTimeText(FString NewText)
{
	GameTimeText->SetText(FText::FromString(NewText));
}

void UUNOverlayWidget::SetGameTimeTextVisibility(bool option)
{
	if (option)
	{
		GameTimeText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		GameTimeText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
