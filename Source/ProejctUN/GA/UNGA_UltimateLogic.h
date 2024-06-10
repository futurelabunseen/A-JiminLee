// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UNGA_UltimateLogic.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNGA_UltimateLogic : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UUNGA_UltimateLogic();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> UltimateActionMontage;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> UltimateSword;

	UFUNCTION(Server, Unreliable)
	void ServerRPCSpawnSword(FVector Location);

public:
	UPROPERTY()
	TObjectPtr<class AUNPlayerCharacter> PlayerCharacter;
};
