// Fill out your copyright notice in the Description page of Project Settings.


#include "../GA/UNGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../GA/AT/UNAT_Trace.h"
#include "../GA/TA/UNTA_Trace.h"
#include "../Attribute/UNCharacterAttributeSet.h"

UUNGA_AttackHitCheck::UUNGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUNGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UUNAT_Trace* AttackTraceTask = UUNAT_Trace::CreateTask(this, AUNTA_Trace::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UUNGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UUNGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UE_LOG(LogTemp, Log, TEXT("Target %s Detected!"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (!SourceASC || !TargetASC)
		{
			UE_LOG(LogTemp, Log, TEXT("Not Have ASC!"));
			return;
		}

		const UUNCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UUNCharacterAttributeSet>();
		UUNCharacterAttributeSet* TargetAttribute = const_cast<UUNCharacterAttributeSet*>(TargetASC->GetSet<UUNCharacterAttributeSet>());
		if (!SourceAttribute|| !TargetAttribute)
		{
			UE_LOG(LogTemp, Log, TEXT("Not Have ATT!"));
			return;
		}

		const float AttackDamage = SourceAttribute->GetAttackRate();
		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
