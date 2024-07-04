// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "UNSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
public:
	UUNSpringArmComponent();

	UFUNCTION()
	void UpdateSpringArmLength(float Start, float End, float Time, float Frame);

	UFUNCTION()
	void ReturnSpringArmLength();

	UFUNCTION()
	void ClearSpringArmTimerHandle();

	UPROPERTY()
	FTimerHandle SpringArmUpdateTimerHandle;

protected:
	UPROPERTY()
	float DefaultSpringArmLength;

	UPROPERTY()
	float SpringArmShortLength;

	UPROPERTY()
	float SpringArmLongLength;

	UPROPERTY()
	float SpringArmStartTime;

	UPROPERTY()
	float SpringArmMoveTime;

};
