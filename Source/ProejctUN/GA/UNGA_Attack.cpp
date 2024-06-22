// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Attack.h"
#include "Character/UNPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/UNComboActionData.h"
#include "AbilitySystemComponent.h"
#include "Tag/UNGameplayTag.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UUNGA_Attack::UUNGA_Attack()
{
	// InstancingPolicy �ɼ� ����
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// GA�� ������ �ִ� Ŭ�������� TryActivateAbility�� �����ϸ� ����Ǵ� �Լ�
void UUNGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUNPlayerCharacter* UNCharacter = CastChecked<AUNPlayerCharacter>(ActorInfo->AvatarActor.Get());
	CurrentComboData = UNCharacter->GetComboActionData();
	UNCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// �ʿ��� ��������Ʈ�� �����ϰ� AT�� ����
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), UNCharacter->GetComboActionMontage(), 1.f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &UUNGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UUNGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	// �޺� Ÿ�̸� ����
	StartComboTimer();
}

// ���� Ű�� ������ �� ���� �޺��� �۵����� üũ
void UUNGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboInput = false;
	}
	else
	{
		HasNextComboInput = true;
	}
}

void UUNGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

// ���� ����Ŭ�� ���� �� �޺� ������ �ʱ�ȭ 
void UUNGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AUNPlayerCharacter* UNCharacter = CastChecked<AUNPlayerCharacter>(ActorInfo->AvatarActor.Get());
	UAbilitySystemComponent* AvatarActorASC = GetAbilitySystemComponentFromActorInfo();

	CurrentComboData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;

	//UNCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	if (!AvatarActorASC->HasMatchingGameplayTag(UNTAG_CHARACTER_STATE_ISSTUNING))
	{
		UNCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUNGA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

// ���� �޺� ���� ����
FName UUNGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);

	return NextSection;
}

// ���� Ű �Է� Ÿ�ְ̹� ������ �������� ���Ͽ� ������ ����ϸ� ���� �޺� ����
void UUNGA_Attack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UUNGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

// ���� �޺� ������ Ȯ���� �� �޺��� ���� �� �� üũ 
void UUNGA_Attack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}
