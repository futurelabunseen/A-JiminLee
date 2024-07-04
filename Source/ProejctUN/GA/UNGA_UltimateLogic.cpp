// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_UltimateLogic.h"
#include "Character/UNPlayerCharacter.h"
#include "Camera/UNSpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ASC/UNAbilitySystemComponent.h"
#include "Tag/UNGameplayTag.h"
#include "Props/UNUltimateSword.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

UUNGA_UltimateLogic::UUNGA_UltimateLogic()
{
	// InstancingPolicy 옵션 변겅
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
	SpringArm = PlayerCharacter->FindComponentByClass<UUNSpringArmComponent>();
	if (!SpringArm)
	{
		return;
	}

	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("UltimateMontage"), UltimateActionMontage, 1.f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UUNGA_UltimateLogic::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UUNGA_UltimateLogic::OnInterruptedCallback);
	
	if (!Sounds.IsEmpty())
	{
		int32 RandomIndex = FMath::RandRange(0, Sounds.Num() - 1);
		USoundBase* Sound = Sounds[RandomIndex];
		UGameplayStatics::PlaySoundAtLocation(this, Sound, PlayerCharacter->GetActorLocation());
	}

	//USkeletalMesh* Mesh = PlayerCharacter->WeaponMesh;
	if (PlayerCharacter->GetOwner()->HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PlayerCharacter;
		FVector SpawnLocation = PlayerCharacter->UltimateLocation + FVector(0.f, 0.f, 750.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		//AUNUltimateSword* Sword = GetWorld()->SpawnActor<AUNUltimateSword>(UltimateSword, SpawnLocation, SpawnRotation, SpawnParams);
		AUNUltimateSword* Sword = GetWorld()->SpawnActorDeferred<AUNUltimateSword>(UltimateSword, FTransform(SpawnRotation, SpawnLocation), PlayerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (Sword)
		{
			Sword->SetMeshToWeaponMesh(PlayerCharacter->WeaponMesh);
			UGameplayStatics::FinishSpawningActor(Sword, FTransform(SpawnRotation, SpawnLocation));
		}
		
		//Sword->SetMeshToWeaponMesh(Mesh);
	}

	//ServerRPCSpawnSword(SpawnLocation);

	SpringArm->UpdateSpringArmLength(800.f, 1600.f, 0.15f, 0.016f);
	//PlayerCharacter->UpdateSpringArmLength(800.f, 1600.f, 0.15f, 0.016f);


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
	if(PlayerCharacter)
	{ 
		SpringArm->UpdateSpringArmLength(1600.f, 800.f, 0.5f, 0.016f);
	}

	UAbilitySystemComponent* AvatarActorASC = GetAbilitySystemComponentFromActorInfo();
	if (PlayerCharacter && AvatarActorASC && !AvatarActorASC->HasMatchingGameplayTag(UNTAG_CHARACTER_STATE_ISSTUNING))
	{

		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
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

// 이 부분은 	if (PlayerCharacter->GetOwner()->HasAuthority())로 대체
void UUNGA_UltimateLogic::ServerRPCSpawnSword_Implementation(FVector Location)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;

	FVector SpawnLocation = Location;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	AUNUltimateSword* Sword = GetWorld()->SpawnActor<AUNUltimateSword>(UltimateSword, SpawnLocation, SpawnRotation, SpawnParams);

	if (PlayerCharacter)
	{
		PlayerCharacter->UltimateLocation = FVector();
	}
}
