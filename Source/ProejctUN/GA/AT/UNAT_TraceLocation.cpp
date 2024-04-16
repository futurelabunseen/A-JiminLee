// Fill out your copyright notice in the Description page of Project Settings.


#include "UNAT_TraceLocation.h"
#include "GA/TA/UNTA_TraceLocation.h"
#include "AbilitySystemComponent.h"

UUNAT_TraceLocation* UUNAT_TraceLocation::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AUNTA_TraceLocation> TargetActorClass)
{
	UUNAT_TraceLocation* NewTask = NewAbilityTask<UUNAT_TraceLocation>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UUNAT_TraceLocation::Activate()
{
	Super::Activate();

	SpawnAndInitalizeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UUNAT_TraceLocation::OnDestroy(bool AbilityEnded)
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

void UUNAT_TraceLocation::SpawnAndInitalizeTargetActor()
{
	SpawnedTargetActor = Cast<AUNTA_TraceLocation>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UUNAT_TraceLocation::OnTargetDataReadyCallback);
		SpawnedTargetActor->CanceledDelegate.AddUObject(this, &UUNAT_TraceLocation::OnTargetDataCancelCallback);
	}
}

// ConfirmTargeting()�� PlayerCharacter�� Ŭ������ ����
void UUNAT_TraceLocation::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		//SpawnedTargetActor->ConfirmTargeting();
	}
}

void UUNAT_TraceLocation::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}
	EndTask();
}

void UUNAT_TraceLocation::OnTargetDataCancelCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		UE_LOG(LogTemp, Log, TEXT("ShouldBroadcastAbilityTaskDelegates"));
		OnDestroy(true);
		OnCanceled.Broadcast(DataHandle);
	}
	UE_LOG(LogTemp, Log, TEXT("Cancel"));
	EndTask();
}
