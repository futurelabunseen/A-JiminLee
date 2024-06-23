// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Teleport.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "TA/UNTA_TraceLocation.h"
#include "AT/UNAT_TraceLocation.h"
#include "Character/UNPlayerCharacter.h"
#include "Tag/UNGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "ASC/UNAbilitySystemComponent.h"

UUNGA_Teleport::UUNGA_Teleport()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// GA를 가지고 있는 클래스에서 TryActivateAbility를 실행하면 실행되는 함수
void UUNGA_Teleport::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	PlayerCharacter = Cast<AUNPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't find Character!"));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}

	if (IsLocallyControlled())
	{
		ActivateDecal();
	}

	UUNAT_TraceLocation* AttackTraceTask = UUNAT_TraceLocation::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UUNGA_Teleport::OnTraceResultCallback);
	AttackTraceTask->OnInterrupted.AddDynamic(this, &UUNGA_Teleport::OnInterruptedCallback);
	AttackTraceTask->OnCanceled.AddDynamic(this, &UUNGA_Teleport::OnCancelCallback);

	AttackTraceTask->ReadyForActivation();
}

void UUNGA_Teleport::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsLocallyControlled())
	{
		if (!bWasCancelled)
		{
			//StartCoolDown();
		}
		EndDecal();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// 반환된 핸들 데이터를 기반으로 캐릭터 텔레포트
void UUNGA_Teleport::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		FVector TargetLocation = HitResult.Location;
		
		if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
		{
			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(TeleportEffect, CurrentEventData.EventMagnitude);
			if (SourceASC && EffectSpecHandle.IsValid())
			{
				//EffectSpecHandle.Data->SetSetByCallerMagnitude(UNTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

				FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
				CueContextHandle.AddInstigator(SourceASC->GetAvatarActor(), SourceASC->GetAvatarActor());
				CueContextHandle.AddHitResult(HitResult);
				
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;
				//CueParam.Location = SourceASC->GetAvatarActor()->GetActorLocation();
				SourceASC->ExecuteGameplayCue(UNTAG_GAMEPLAYCUE_CHARACTER_TELEPORTEFFECT, CueParam);

				PlayerCharacter->GetController()->StopMovement();
				TeleportToLocation(TargetLocation, CueParam);
				StartCoolDown();
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Location is null!"));
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
	//bool bReplicatedEndAbility = false;
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

// Decal을 이용한 범위 표시
void UUNGA_Teleport::ActivateDecal()
{
	PlayerCharacter->ActivateDecal(DecalStruct);
}

void UUNGA_Teleport::EndDecal()
{
	PlayerCharacter->EndDecal();
}

void UUNGA_Teleport::TeleportToLocation_Implementation(FVector NewLocation, FGameplayCueParameters Params)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
	Params.Location = SourceASC->GetAvatarActor()->GetActorLocation();
	SourceASC->ExecuteGameplayCue(UNTAG_GAMEPLAYCUE_CHARACTER_TELEPORTEFFECT, Params);

	PlayerCharacter->TeleportTo(NewLocation, (NewLocation - PlayerCharacter->GetActorLocation()).Rotation(), false, true);
}

void UUNGA_Teleport::StartCoolDown_Implementation()
{
	CommitAbilityCooldown(FGameplayAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfoRef(), false);
}
