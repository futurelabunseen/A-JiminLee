// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UNGA_Skill.generated.h"

/**
 * 
 */
class UUNAbilitySystemComponent;
class UCharacterMovementComponent;
class UAnimMontage;

UCLASS()
class PROEJCTUN_API UUNGA_Skill : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UUNGA_Skill();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	UPROPERTY()
	TObjectPtr<UAnimMontage> ActiveSkillActionMontage;

	UPROPERTY()
	TObjectPtr<UUNAbilitySystemComponent> SourceASC;

	UPROPERTY()
	TObjectPtr<AActor> AvatarActor;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComp;
};