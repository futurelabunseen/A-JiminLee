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
	// InstancingPolicy �ɼ� ����
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// GA�� ������ �ִ� Ŭ�������� TryActivateAbility�� �����ϸ� ����Ǵ� �Լ�
void UUNGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLevel = CurrentEventData.EventMagnitude;

	// �ʿ��� ��������Ʈ�� �����ϰ� AT�� ����
	UUNAT_Trace* AttackTraceTask = UUNAT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UUNGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UUNGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// ���� ���� ��
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

			// �ǰ� ĳ������ Attribute�� �������� �ڵ�. �ʿ� �� �ּ� ���� �� ��� ����.

			//const UUNCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UUNCharacterAttributeSet>();
			//UUNCharacterAttributeSet* TargetAttribute = const_cast<UUNCharacterAttributeSet*>(TargetASC->GetSet<UUNCharacterAttributeSet>());
			//if (!SourceAttribute|| !TargetAttribute)
			//{
			//	UE_LOG(LogTemp, Log, TEXT("Not Have ATT!"));
			//	return;
			//}
			//const float AttackDamage = SourceAttribute->GetAttackRate();
			//TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);

			// GE�� ���ؼ� ������ ����
			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
			if (EffectSpecHandle.IsValid())
			{
				//EffectSpecHandle.Data->SetSetByCallerMagnitude(UNTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

				FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
				CueContextHandle.AddHitResult(HitResult);
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;

				TargetASC->ExecuteGameplayCue(UNTAG_GameplayCue_CHARACTER_AttackHit, CueParam);
			}
		}
	}
	// ���� ���� ���� ��
	else if (TargetDataHandle.UniqueId == 1)
	{
		//FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
		{
			// GE�� ���ؼ� ���ϰ� ������ ����
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

				//SourceASC->ExecuteGameplayCue(UNTAG_GameplayCue_CHARACTER_AttackHit, CueParam);
				SourceASC->ExecuteGameplayCue(UNTAG_GAMEPLAYCUE_CHARACTER_FLOORSKILLEFFECT, CueParam);
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
