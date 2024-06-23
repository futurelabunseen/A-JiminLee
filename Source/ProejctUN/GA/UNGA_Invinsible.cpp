// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Invinsible.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ASC/UNAbilitySystemComponent.h"
#include "Tag/UNGameplayTag.h"

UUNGA_Invinsible::UUNGA_Invinsible()
{
	// InstancingPolicy ¿É¼Ç º¯°Ï
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UUNGA_Invinsible::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(InvinsibleEffect, CurrentEventData.EventMagnitude);

	if (SourceASC && EffectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);

		FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
		CueContextHandle.AddInstigator(SourceASC->GetAvatarActor(), SourceASC->GetAvatarActor());

		FGameplayCueParameters CueParam;
		CueParam.EffectContext = CueContextHandle;
		SourceASC->ExecuteGameplayCue(UNTAG_GAMEPLAYCUE_CHARACTER_INVINSIBLEEFFECT, CueParam);

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}

	//FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UUNGA_Invinsible::DelayFunction, Handle, ActorInfo, ActivationInfo), 5.0f, false);
}

void UUNGA_Invinsible::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

//void UUNGA_Invinsible::DelayFunction(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
//}
