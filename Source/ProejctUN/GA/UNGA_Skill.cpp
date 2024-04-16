// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Skill.h"
#include "Character/UNPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UUNGA_Skill::UUNGA_Skill()
{
	// InstancingPolicy �ɼ� ����
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// GA�� ������ �ִ� Ŭ�������� TryActivateAbility�� �����ϸ� ����Ǵ� �Լ�
void UUNGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (!PlayerCharacter)
	{
		return;
	}

	ActiveSkillActionMontage = PlayerCharacter->GetSkillActionMontage();
	if (!ActiveSkillActionMontage)
	{
		return;
	}

	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// �ʿ��� ��������Ʈ�� �����ϰ� AT�� ����
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SKillMontage"), ActiveSkillActionMontage, 1.f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UUNGA_Skill::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UUNGA_Skill::OnInterruptedCallback);
	PlayMontageTask->ReadyForActivation();
}

void UUNGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
	CommitAbilityCooldown(FGameplayAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfoRef(), false);

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
