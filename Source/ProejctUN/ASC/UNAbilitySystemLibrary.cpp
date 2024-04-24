// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/UNAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/UNGASPlayerState.h"
#include "Attribute/UNCharacterAttributeSet.h"
#include "UI/WC/UNWidgetController.h"
#include "UI/UNHUD.h"

UUNOverlayWidgetController* UUNAbilitySystemLibrary::GetoverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AUNHUD* UNHUD = Cast<AUNHUD>(PC->GetHUD()))
		{
			AUNGASPlayerState* PS = PC->GetPlayerState<AUNGASPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return UNHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UUNGASInventoryWidgetController* UUNAbilitySystemLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AUNHUD* UNHUD = Cast<AUNHUD>(PC->GetHUD()))
		{
			AUNGASPlayerState* PS = PC->GetPlayerState<AUNGASPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return UNHUD->GetInventoryWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
