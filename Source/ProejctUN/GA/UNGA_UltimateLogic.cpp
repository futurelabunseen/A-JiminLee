// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_UltimateLogic.h"
#include "Character/UNPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ASC/UNAbilitySystemComponent.h"
#include "Tag/UNGameplayTag.h"
#include "Props/UNUltimateSword.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

UUNGA_UltimateLogic::UUNGA_UltimateLogic()
{
	// InstancingPolicy ¿É¼Ç º¯°Ï
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUNGA_UltimateLogic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	PlayerCharacter = Cast<AUNPlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (!PlayerCharacter)
	{
		return;
	}

	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	if (UUNAbilitySystemComponent* ASC = Cast<UUNAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Checked()))
	{
		ASC->SetCurrentActiveAbility(this);
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("UltimateMontage"), UltimateActionMontage, 1.f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UUNGA_UltimateLogic::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UUNGA_UltimateLogic::OnInterruptedCallback);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;

	FVector SpawnLocation = PlayerCharacter->UltimateLocation + FVector(0.f, 0.f, 1000.f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	AUNUltimateSword* Sword = GetWorld()->SpawnActor<AUNUltimateSword>(UltimateSword, SpawnLocation, SpawnRotation, SpawnParams);

	//ServerRPCSpawnSword(SpawnLocation);
	PlayerCharacter->UpdateSpringArmLength(800.f, 1600.f, 1.f);
	PlayMontageTask->ReadyForActivation();
}

void UUNGA_UltimateLogic::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_UltimateLogic::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PlayerCharacter->UpdateSpringArmLength(1600.f, 800.f, 0.5f);
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUNGA_UltimateLogic::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_UltimateLogic::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_UltimateLogic::ServerRPCSpawnSword_Implementation(FVector Location)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;

	FVector SpawnLocation = Location + FVector(0.f, 0.f, 1000.f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	AUNUltimateSword* Sword = GetWorld()->SpawnActor<AUNUltimateSword>(UltimateSword, SpawnLocation, SpawnRotation, SpawnParams);

	PlayerCharacter->UltimateLocation = FVector();
}
