// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Teleport.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "TA/UNTA_TraceLocation.h"
#include "AT/UNAT_TraceLocation.h"
#include "Character/UNPlayerCharacter.h"

UUNGA_Teleport::UUNGA_Teleport()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// GA�� ������ �ִ� Ŭ�������� TryActivateAbility�� �����ϸ� ����Ǵ� �Լ�
void UUNGA_Teleport::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UUNAT_TraceLocation* TraceLocation = UUNAT_TraceLocation::CreateTask(this, TargetActorClass);
	
	ActivateDecal();
	UUNAT_TraceLocation* AttackTraceTask = UUNAT_TraceLocation::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UUNGA_Teleport::OnTraceResultCallback);
	AttackTraceTask->OnInterrupted.AddDynamic(this, &UUNGA_Teleport::OnInterruptedCallback);
	AttackTraceTask->OnCanceled.AddDynamic(this, &UUNGA_Teleport::OnCancelCallback);
	AttackTraceTask->ReadyForActivation();
}

//void UUNGA_Teleport::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
//{
//	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
//
//	EndDecal();
//	bool bReplicateEndAbility = false;
//	bool bWasCancelled = true;
//	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}

// ��ȯ�� �ڵ� �����͸� ������� ĳ���� �ڷ���Ʈ
void UUNGA_Teleport::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		FVector TargetLocation = HitResult.Location;

		AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
		if (PlayerCharacter)
		{
			PlayerCharacter->GetController()->StopMovement();
			PlayerCharacter->TeleportTo(TargetLocation, (TargetLocation - PlayerCharacter->GetActorLocation()).Rotation(), false, true);
		}

		CommitAbilityCooldown(FGameplayAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfoRef(), false);
		EndDecal();

		bool bReplicatedEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
	}
}

void UUNGA_Teleport::OnInterruptedCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_Teleport::OnCancelCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	EndDecal();
	bool bReplicatedEndAbility = false;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

// Decal�� �̿��� ���� ǥ��
void UUNGA_Teleport::ActivateDecal()
{
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
	if (PlayerCharacter)
	{
		PlayerCharacter->ActivateDecal(DecalStruct);
	}
}

void UUNGA_Teleport::EndDecal()
{
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
	if (PlayerCharacter)
	{
		PlayerCharacter->EndDecal();
	}
}