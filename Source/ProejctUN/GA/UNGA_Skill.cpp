// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Skill.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ASC/UNAbilitySystemComponent.h"
#include "Tag/UNGameplayTag.h"


UUNGA_Skill::UUNGA_Skill()
{
	// InstancingPolicy 옵션 변겅
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// GA를 가지고 있는 클래스에서 TryActivateAbility를 실행하면 실행되는 함수
void UUNGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AvatarActor = ActorInfo->AvatarActor.Get();
	#pragma region AvatarActor NullCheck & return
	if (!AvatarActor)
	{
		UE_LOG(LogTemp, Log, TEXT("AvatarActor is Null!"));
		return;
	}
#pragma endregion

	SourceASC = Cast<UUNAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Checked());
	#pragma region SourceASC NullCheck & return
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Log, TEXT("SourceASC is Null!"));
		return;
	}
#pragma endregion

	ActiveSkillActionMontage = SourceASC->GetSkillActionMontage();
	#pragma region ActiveSkillActionMontage NullCheck & return
		if (!ActiveSkillActionMontage)
		{
			UE_LOG(LogTemp, Log, TEXT("ActiveSkillActionMontage is Null!"));
			return;
		}
	#pragma endregion

	MovementComp = AvatarActor->FindComponentByClass<UCharacterMovementComponent>();
	#pragma region MovementComp NullCheck & return
		if (!MovementComp)
		{
			UE_LOG(LogTemp, Log, TEXT("MovementComp is Null!"));
			return;
		}
	#pragma endregion

	MovementComp->SetMovementMode(EMovementMode::MOVE_None);

	// 스킬 캔슬을 위한 어빌리티 등록
	if (UUNAbilitySystemComponent* ASC = Cast<UUNAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Checked()))
	{
		ASC->SetCurrentActiveAbility(this);
	}

	// 필요한 델리게이트를 지정하고 AT를 실행
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SKillMontage"), ActiveSkillActionMontage, 1.f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UUNGA_Skill::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UUNGA_Skill::OnInterruptedCallback);
	PlayMontageTask->ReadyForActivation();
}

void UUNGA_Skill::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (UUNAbilitySystemComponent* ASC = Cast<UUNAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Checked()))
	{
		ASC->SetCurrentActiveAbility(nullptr);
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	//if (UUNAbilitySystemComponent* ASC = Cast<UUNAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Checked()))
	//{
	//	FGameplayTagContainer Container;
	//	Container.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.State.SkillCoolDown")));
	//	Container.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.State.IsSkilling")));
	//	ASC->RemoveActiveEffectsWithTags(Container);
	//}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AvatarActor && !SourceASC->HasMatchingGameplayTag(UNTAG_CHARACTER_STATE_ISSTUNING))
	{
		MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	if (bWasCancelled == false)
	{
		CommitAbilityCooldown(FGameplayAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfoRef(), false);
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
