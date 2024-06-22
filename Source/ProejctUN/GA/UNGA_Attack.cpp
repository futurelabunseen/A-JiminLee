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
	// InstancingPolicy 옵션 변겅
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// GA를 가지고 있는 클래스에서 TryActivateAbility를 실행하면 실행되는 함수
void UUNGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUNPlayerCharacter* UNCharacter = CastChecked<AUNPlayerCharacter>(ActorInfo->AvatarActor.Get());
	CurrentComboData = UNCharacter->GetComboActionData();
	UNCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 필요한 델리게이트를 지정하고 AT를 실행
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), UNCharacter->GetComboActionMontage(), 1.f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &UUNGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UUNGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	// 콤보 타이머 실행
	StartComboTimer();
}

// 공격 키가 눌렸을 때 다음 콤보가 작동될지 체크
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

// 공격 사이클이 끝날 시 콤보 데이터 초기화 
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

// 다음 콤보 섹션 진행
FName UUNGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);

	return NextSection;
}

// 공격 키 입력 타이밍과 설정한 프레임을 비교하여 조건을 통과하면 다음 콤보 실행
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

// 다음 콤보 진행이 확정된 뒤 콤보가 언제 들어갈 지 체크 
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
