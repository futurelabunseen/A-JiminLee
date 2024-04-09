// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Skill.h"
#include "Character/UNPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UUNGA_Skill::UUNGA_Skill()
{
	// InstancingPolicy 옵션 변겅
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// GA를 가지고 있는 클래스에서 TryActivateAbility를 실행하면 실행되는 함수
void UUNGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUNPlayerCharacter* TargetCharacter = Cast<AUNPlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (!TargetCharacter)
	{
		return;
	}

	ActiveSkillActionMontage = TargetCharacter->GetSkillActionMontage();
	if (!ActiveSkillActionMontage)
	{
		return;
	}

	TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 필요한 델리게이트를 지정하고 AT를 실행
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SKillMontage"), ActiveSkillActionMontage, 1.f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UUNGA_Skill::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UUNGA_Skill::OnInterruptedCallback);

	PlayMontageTask->ReadyForActivation();
}

void UUNGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AUNPlayerCharacter* TargetCharacter = Cast<AUNPlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (TargetCharacter)
	{
		TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUNGA_Skill::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_Skill::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
