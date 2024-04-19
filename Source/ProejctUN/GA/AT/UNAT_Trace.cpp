// Fill out your copyright notice in the Description page of Project Settings.


#include "UNAT_Trace.h"
#include "GA/TA/UNTA_Trace.h"
#include "AbilitySystemComponent.h"

// ���ο� �����Ƽ �½�ũ�� ����� TA�� �Ҵ�
UUNAT_Trace* UUNAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AUNTA_Trace> TargetActorClass)
{
	UUNAT_Trace* NewTask = NewAbilityTask<UUNAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

// AT ����
void UUNAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitalizeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

// AT�� Destroy�ɶ� ����
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

// TA�� ���� �� ��������Ʈ�� �Լ� ���
void UUNAT_Trace::SpawnAndInitalizeTargetActor()
{
	SpawnedTargetActor = Cast<AUNTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UUNAT_Trace::OnTargetDataReadyCallback);
	}
}

// TA �۵�
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

// ��������Ʈ�� �۵��Ǵ� �Լ�
void UUNAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
