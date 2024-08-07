// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/UNCharacterAttributeSet.h"
#include "ASC/UNAbilitySystemComponent.h"
#include "Tag/UNGameplayTag.h"
#include "GA/AT/UNAT_Trace.h"
#include "GA/TA/UNTA_Trace.h"

UUNGA_AttackHitCheck::UUNGA_AttackHitCheck()
{
	// InstancingPolicy 옵션 변겅
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// GA를 가지고 있는 클래스에서 TryActivateAbility를 실행하면 실행되는 함수
void UUNGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UUNAbilitySystemComponent* ASC = Cast<UUNAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Checked()))
	{
		ASC->SetCurrentActiveAbility(nullptr);
		ASC->bAbilityAlreadyActivate = true;
	}

	CurrentLevel = CurrentEventData.EventMagnitude;

	// 필요한 델리게이트를 지정하고 AT를 실행
	UUNAT_Trace* AttackTraceTask = UUNAT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UUNGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UUNGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 단일 공격 시
	if (TargetDataHandle.UniqueId == 0)
	{
		if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
		{
			FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
			//UE_LOG(LogTemp, Log, TEXT("Target %s Detected!"), *(HitResult.GetActor()->GetName()));

			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

			if (!SourceASC || !TargetASC)
			{
				UE_LOG(LogTemp, Log, TEXT("Not Have ASC!"));

				bool bReplicatedEndAbility = true;
				bool bWasCancelled = true;
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
				return;
			}

			// 피격 캐릭터의 Attribute를 가져오는 코드. 필요 시 주석 제거 후 사용 예정.

			//const UUNCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UUNCharacterAttributeSet>();
			//UUNCharacterAttributeSet* TargetAttribute = const_cast<UUNCharacterAttributeSet*>(TargetASC->GetSet<UUNCharacterAttributeSet>());
			//if (!SourceAttribute|| !TargetAttribute)
			//{
			//	UE_LOG(LogTemp, Log, TEXT("Not Have ATT!"));
			//	return;
			//}
			//const float AttackDamage = SourceAttribute->GetAttackRate();
			//TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);

			// GE를 통해서 데미지 전달
			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
			if (EffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

				FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
				CueContextHandle.AddHitResult(HitResult);
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;

				TargetASC->ExecuteGameplayCue(UNTAG_GameplayCue_CHARACTER_AttackHit, CueParam);
			}
		}
	}
	// 광역 스턴 공격 시
	else if (TargetDataHandle.UniqueId == 1)
	{
		//FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
		{
			// GE를 통해서 스턴과 데미지 전달
			FGameplayEffectSpecHandle StunEffectSpecHandle = MakeOutgoingGameplayEffectSpec(StunEffect, CurrentLevel);
			FGameplayEffectSpecHandle AttackEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
			if (AttackEffectSpecHandle.IsValid() && StunEffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, StunEffectSpecHandle, TargetDataHandle);
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, AttackEffectSpecHandle, TargetDataHandle);

				FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(AttackEffectSpecHandle);
				CueContextHandle.AddInstigator(SourceASC->GetAvatarActor(), SourceASC->GetAvatarActor());
				CueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;

				SourceASC->ExecuteGameplayCue(UNTAG_GAMEPLAYCUE_CHARACTER_FLOORSKILLEFFECT, CueParam);

				FGameplayEffectContextHandle StunCueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(StunEffectSpecHandle);
				StunCueContextHandle.AddInstigator(SourceASC->GetAvatarActor(), SourceASC->GetAvatarActor());
				StunCueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
				FGameplayCueParameters StunCueParam;
				StunCueParam.EffectContext = StunCueContextHandle;

				SourceASC->ExecuteGameplayCue(UNTAG_GAMEPLAYCUE_CHARACTER_STUNEFFECT, StunCueParam);
			}
		}
		else
		{
			FGameplayCueParameters CueParam;
			CueParam.Instigator = SourceASC->GetAvatarActor();
			SourceASC->ExecuteGameplayCue(UNTAG_GAMEPLAYCUE_CHARACTER_FLOORSKILLEFFECT, CueParam);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
