// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_UltimateLogic.h"

#include "ASC/UNAbilitySystemComponent.h"
#include "Tag/UNGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"

#include "Camera/UNSpringArmComponent.h"
#include "Props/UNUltimateSword.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

UUNGA_UltimateLogic::UUNGA_UltimateLogic()
{
	// InstancingPolicy 옵션 변겅
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUNGA_UltimateLogic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AvatarActor = ActorInfo->AvatarActor.Get();
	#pragma region AvatarActor NullCheck & return
	if (!AvatarActor)
	{
		UE_LOG(LogTemp, Log, TEXT("AvatarActor is Null!"));
		return;
	}
#pragma endregion

	SpringArm = AvatarActor->FindComponentByClass<UUNSpringArmComponent>();
	#pragma region SpringArm NullCheck & return
	if (!SpringArm)
	{
		UE_LOG(LogTemp, Log, TEXT("SpringArm is Null!"));
		return;
	}
#pragma endregion

	SourceASC = Cast<UUNAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Checked());
	#pragma region SourceASC NullCheck & return
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Log, TEXT("SourceASC is Null!"));
		return;
	}
#pragma endregion

	MovementComp = AvatarActor->FindComponentByClass<UCharacterMovementComponent>();
	#pragma region MovementComp NullCheck & return
		if (!MovementComp)
		{
			UE_LOG(LogTemp, Log, TEXT("MovementComp is Null!"));
			return;
		}
	#pragma endregion

	MovementComp->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("UltimateMontage"), UltimateActionMontage, 1.f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UUNGA_UltimateLogic::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UUNGA_UltimateLogic::OnInterruptedCallback);
	
	if (!Sounds.IsEmpty())
	{
		int32 RandomIndex = FMath::RandRange(0, Sounds.Num() - 1);
		USoundBase* Sound = Sounds[RandomIndex];
		UGameplayStatics::PlaySoundAtLocation(this, Sound, AvatarActor->GetActorLocation());
	}

	//USkeletalMesh* Mesh = PlayerCharacter->WeaponMesh;
	if (AvatarActor->GetOwner()->HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = AvatarActor;
		FVector SpawnLocation = SourceASC->GetUltimateLocation() + FVector(0.f, 0.f, 750.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		//AUNUltimateSword* Sword = GetWorld()->SpawnActor<AUNUltimateSword>(UltimateSword, SpawnLocation, SpawnRotation, SpawnParams);
		AUNUltimateSword* Sword = GetWorld()->SpawnActorDeferred<AUNUltimateSword>(UltimateSword, FTransform(SpawnRotation, SpawnLocation), AvatarActor, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (Sword)
		{
			Sword->SetMeshToWeaponMesh(SourceASC->GetUltimateMesh());
			//Sword->SetMeshToWeaponMesh(PlayerCharacter->WeaponMesh);
			UGameplayStatics::FinishSpawningActor(Sword, FTransform(SpawnRotation, SpawnLocation));
		}
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
	if(AvatarActor)
	{ 
		SpringArm->UpdateSpringArmLength(1600.f, 800.f, 0.5f, 0.016f);
	}

	//UAbilitySystemComponent* AvatarActorASC = GetAbilitySystemComponentFromActorInfo();
	if (AvatarActor && SourceASC && !SourceASC->HasMatchingGameplayTag(UNTAG_CHARACTER_STATE_ISSTUNING))
	{

		MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
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
	SpawnParams.Owner = AvatarActor;

	FVector SpawnLocation = Location;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	AUNUltimateSword* Sword = GetWorld()->SpawnActor<AUNUltimateSword>(UltimateSword, SpawnLocation, SpawnRotation, SpawnParams);

	if (SourceASC)
	{
		SourceASC->SetUltimateLocation(FVector());
	}
}
