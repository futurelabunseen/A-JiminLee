// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/UNCharacter.h"
#include "AbilitySystemInterface.h"
#include "UNPlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNPlayerCharacter : public AUNCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AUNPlayerCharacter();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;


protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay();

	virtual void PossessedBy(AController* NewController) override;

	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

private:
	FVector CachedDestination;

	float FollowTime;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere)
	APlayerController* PlayerController;
};
