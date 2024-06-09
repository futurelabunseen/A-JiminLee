// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Ultimate.h"

UUNGA_Ultimate::UUNGA_Ultimate()
{
	// InstancingPolicy ¿É¼Ç º¯°Ï
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUNGA_Ultimate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, "Ultimate");
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UUNGA_Ultimate::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
