// Fill out your copyright notice in the Description page of Project Settings.


#include "UNAT_Trace.h"
#include "GA/TA/UNTA_Trace.h"
#include "AbilitySystemComponent.h"

// 새로운 어빌리티 태스크를 만들고 TA를 할당
UUNAT_Trace* UUNAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AUNTA_Trace> TargetActorClass)
{
	UUNAT_Trace* NewTask = NewAbilityTask<UUNAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

// AT 실행
void UUNAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitalizeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

// AT가 Destroy될때 실행
void UUNAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
		if (ASC)
		{
			ASC->SpawnedTargetActors.Remove(SpawnedTargetActor);
		}

		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

// TA를 스폰 후 델리게이트에 함수 등록
void UUNAT_Trace::SpawnAndInitalizeTargetActor()
{
	SpawnedTargetActor = Cast<AUNTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UUNAT_Trace::OnTargetDataReadyCallback);
	}
}

// TA 작동
void UUNAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

// 델리게이트로 작동되는 함수
void UUNAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
