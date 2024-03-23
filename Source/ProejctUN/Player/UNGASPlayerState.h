// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "UNGASPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AUNGASPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;
};
