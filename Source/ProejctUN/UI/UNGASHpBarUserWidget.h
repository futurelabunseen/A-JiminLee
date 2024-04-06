// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/UNGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "UNGASHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNGASHpBarUserWidget : public UUNGASUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnHealthChange(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChange(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChange(const FGameplayTag CallbackTag, int32 NewCount);
	void UpdateHpBar();

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	float CurrentHealth = 0.f;
	float CurrentMaxHealth = 0.1f;

	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvinsibleColor = FLinearColor::Blue;
};
