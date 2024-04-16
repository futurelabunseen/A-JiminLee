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

// GA를 가지고 있는 클래스에서 TryActivateAbility를 실행하면 실행되는 함수
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

void UUNGA_Teleport::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!bWasCancelled)
	{
		StartCoolDown();
	}
	EndDecal();

	UE_LOG(LogTemp, Log, TEXT("EndTeleport"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// 반환된 핸들 데이터를 기반으로 캐릭터 텔레포트
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
			PlayerCharacter->TeleportToLocation(TargetLocation);
		}

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
	UE_LOG(LogTemp, Log, TEXT("OnCancelCallback"));
	bool bReplicatedEndAbility = false;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

// Decal을 이용한 범위 표시
void UUNGA_Teleport::ActivateDecal()
{
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("ActivateDecal"));
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

void UUNGA_Teleport::StartCoolDown()
{
	CommitAbilityCooldown(FGameplayAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfoRef(), true);
}
