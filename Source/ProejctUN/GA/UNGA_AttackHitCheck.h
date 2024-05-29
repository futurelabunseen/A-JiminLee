// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UNGA_AttackHitCheck.generated.h"

class UGameplayEffect;
class AUNTA_Trace;
/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UUNGA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

protected:

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<UGameplayEffect> StunEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<AUNTA_Trace> TargetActorClass;

	float CurrentLevel;
};
