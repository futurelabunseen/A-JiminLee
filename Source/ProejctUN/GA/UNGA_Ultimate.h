// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Struct/DecalStruct.h"
#include "UNGA_Ultimate.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNGA_Ultimate : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UUNGA_Ultimate();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

// Decal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Struct)
	FDecalStruct DecalStruct;

	UFUNCTION()
	void ActivateDecal();

	UFUNCTION()
	void EndDecal();

	UFUNCTION(Server, Unreliable)
	void ServerRPCSendHitLocation(FVector Location);

protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void OnInterruptedCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void OnCancelCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class AUNTA_TraceLocation> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AUNPlayerCharacter> PlayerCharacter;
};
