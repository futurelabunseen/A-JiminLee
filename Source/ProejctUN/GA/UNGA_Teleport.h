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
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void OnInterruptedCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void OnCancelCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void ActivateDecal();

	UFUNCTION()
	void EndDecal();

	UFUNCTION(Server, reliable, WithValidation)
	void ServerRPCTeleportToLocation(FVector NewLocation, FGameplayCueParameters Params);

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class AUNTA_TraceLocation> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AActor> AvatarActor;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> TeleportEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector HalfCapsuleVector;

public:
	UFUNCTION(Server, Unreliable)
	void StartCoolDown();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Struct)
	FDecalStruct DecalStruct;
};
