// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Ultimate.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TA/UNTA_TraceLocation.h"
#include "AT/UNAT_TraceLocation.h"
#include "Character/UNPlayerCharacter.h"

#include "Tag/UNGameplayTag.h"
#include "ASC/UNAbilitySystemComponent.h"

#include "Kismet/GameplayStatics.h"


UUNGA_Ultimate::UUNGA_Ultimate()
{
	// InstancingPolicy ¿É¼Ç º¯°Ï
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUNGA_Ultimate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
	AttackTraceTask->OnComplete.AddDynamic(this, &UUNGA_Ultimate::OnTraceResultCallback);
	AttackTraceTask->OnInterrupted.AddDynamic(this, &UUNGA_Ultimate::OnInterruptedCallback);
	AttackTraceTask->OnCanceled.AddDynamic(this, &UUNGA_Ultimate::OnCancelCallback);

	AttackTraceTask->ReadyForActivation();
	//GetWorld()->GetTimerManager().SetTimer(SpringArmUpdateTimerHandle, TimerCallback, 0.01f, true);
	//EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UUNGA_Ultimate::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	EndDecal();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUNGA_Ultimate::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		FVector TargetLocation = HitResult.Location;
		if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
		{
			PlayerCharacter->UltimateLocation = TargetLocation;
			ServerRPCSendHitLocation(TargetLocation);
			//DrawDebugSphere(GetWorld(), TargetLocation, 25.0f, 12, FColor::Red, false, 2.0f);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_Ultimate::OnInterruptedCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_Ultimate::OnCancelCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_Ultimate::ActivateDecal()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->ActivateDecal_Implementation(DecalStruct);
	}
}

void UUNGA_Ultimate::EndDecal()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->EndDecal_Implementation();
	}
}

void UUNGA_Ultimate::ServerRPCSendHitLocation_Implementation(FVector Location)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->StartUltimate(Location);
	}
}
