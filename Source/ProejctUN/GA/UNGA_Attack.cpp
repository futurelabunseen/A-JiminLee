// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Attack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/UNComboActionData.h"
#include "AbilitySystemComponent.h"
#include "Tag/UNGameplayTag.h"
#include "ASC/UNAbilitySystemComponent.h"

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

	MovementComp = AvatarActor->FindComponentByClass<UCharacterMovementComponent>();
	#pragma region MovementComp NullCheck & return
	if (!MovementComp)
	{
		UE_LOG(LogTemp, Log, TEXT("MovementComp is Null!"));
		return;
	}
#pragma endregion

	CurrentComboData = SourceASC->GetComboActionData();
	MovementComp->SetMovementMode(EMovementMode::MOVE_None);

	// �ʿ��� ��������Ʈ�� �����ϰ� AT�� ����
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), SourceASC->GetComboActionMontage(), 1.f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &UUNGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UUNGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	// �޺� Ÿ�̸� ����
	StartComboTimer();

	//CancelAbility(Handle, ActorInfo, ActivationInfo, true);
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
	CurrentComboData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;

	if (AvatarActor && !SourceASC->HasMatchingGameplayTag(UNTAG_CHARACTER_STATE_ISSTUNING))
	{
		MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
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
