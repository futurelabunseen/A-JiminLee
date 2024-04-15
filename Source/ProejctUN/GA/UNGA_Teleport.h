// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "Struct/DecalStruct.h"
#include "UNGA_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNGA_Teleport : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UUNGA_Teleport();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void ActivateDecal();

	UFUNCTION()
	void EndDecal();

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class AUNTA_TraceLocation> TargetActorClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Struct)
	FDecalStruct DecalStruct;
};
