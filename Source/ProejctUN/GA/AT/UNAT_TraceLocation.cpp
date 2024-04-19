// Fill out your copyright notice in the Description page of Project Settings.


#include "UNAT_TraceLocation.h"
#include "GA/TA/UNTA_TraceLocation.h"
#include "AbilitySystemComponent.h"
#include "UNAbilitySystemComponent.h"

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
		UUNAbilitySystemComponent* ASC = CastChecked<UUNAbilitySystemComponent>(AbilitySystemComponent.Get());
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

// ConfirmTargeting()은 PlayerCharacter의 클릭으로 진행
void UUNAT_TraceLocation::FinalizeTargetActor()
{
	UUNAbilitySystemComponent* ASC = CastChecked<UUNAbilitySystemComponent>(AbilitySystemComponent.Get());
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);
		ASC->SetCurrentActiveTargetActor(SpawnedTargetActor);

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
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC)
	{
		return;
	}

	FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());

	if (IsPredictingClient())
	{
		if (!SpawnedTargetActor->ShouldProduceTargetDataOnServer)
		{
			ASC->ServerSetReplicatedTargetDataCancelled(GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
		}
		else
		{
			// We aren't going to send the target data, but we will send a generic confirmed message.
			ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::GenericCancel, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCanceled.Broadcast(DataHandle);
	}
	EndTask();
}
