// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UNPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */


UCLASS()
class PROEJCTUN_API AUNPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AUNPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

protected:

	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	virtual void BeginPlay();

	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

private:
	FVector CachedDestination;

	float FollowTime;
};
