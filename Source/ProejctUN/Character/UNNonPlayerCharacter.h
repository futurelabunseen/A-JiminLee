// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/UNCharacter.h"
#include "AbilitySystemInterface.h"
#include "Engine/StreamableManager.h"
#include "UNNonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNNonPlayerCharacter : public AUNCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AUNNonPlayerCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UUNCharacterAttributeSet> AttributeSet;
};
