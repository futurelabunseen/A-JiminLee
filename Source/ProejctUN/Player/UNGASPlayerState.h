// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "UNGASPlayerState.generated.h"

class UUNAbilitySystemComponent;
class UUNCharacterAttributeSet;
class UUNCharacterSkillAttributeSet;
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

	UUNCharacterAttributeSet* GetAttributeSet();
protected:
	UPROPERTY()
	TObjectPtr<UUNAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UUNCharacterAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<UUNCharacterSkillAttributeSet> SkillAttributeSet;

};
