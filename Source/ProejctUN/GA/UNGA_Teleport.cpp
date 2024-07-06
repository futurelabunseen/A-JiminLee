// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/UNGA_Teleport.h"
#include "ASC/UNAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "TA/UNTA_TraceLocation.h"
#include "AT/UNAT_TraceLocation.h"

#include "Interface/DecalSystemInterface.h"
#include "NavigationSystem.h"

#include "Tag/UNGameplayTag.h"

UUNGA_Teleport::UUNGA_Teleport()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	HalfCapsuleVector = FVector(0.f, 0.f, 50.f);
}

// GA를 가지고 있는 클래스에서 TryActivateAbility를 실행하면 실행되는 함수
void UUNGA_Teleport::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AvatarActor = CurrentActorInfo->AvatarActor.Get();
	#pragma region AvatarActor NullCheck & return
	if (!AvatarActor)
	{
		UE_LOG(LogTemp, Log, TEXT("AvatarActor is Null!"));
		return;
	}
#pragma endregion
	#pragma region InterfaceImplements NullCheck & return
		if (!AvatarActor->GetClass()->ImplementsInterface(UDecalSystemInterface::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("SourceActor Not Implements DecalInterface!"));
			return;
		}
#pragma endregion

	SourceInterface = TScriptInterface<IDecalSystemInterface>(AvatarActor);
	if (!SourceInterface)
	{
		UE_LOG(LogTemp, Log, TEXT("Not Have DecalInterface!"));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
	
	if (IsLocallyControlled())
	{
		ActivateDecal();
	}

	UUNAT_TraceLocation* AttackTraceTask = UUNAT_TraceLocation::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UUNGA_Teleport::OnTraceResultCallback);
	AttackTraceTask->OnInterrupted.AddDynamic(this, &UUNGA_Teleport::OnInterruptedCallback);
	AttackTraceTask->OnCanceled.AddDynamic(this, &UUNGA_Teleport::OnCancelCallback);

	AttackTraceTask->ReadyForActivation();
}

void UUNGA_Teleport::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsLocallyControlled())
	{
		if (!bWasCancelled)
		{
			//StartCoolDown();
		}
		EndDecal();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// 반환된 핸들 데이터를 기반으로 캐릭터 텔레포트
void UUNGA_Teleport::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		// 반환된 위치 데이터 캐싱
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		FVector TargetLocation = HitResult.Location;

		// 근처 네비게이션포인트로 위치값 변경 시도
		UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (Nav)
		{
			// Hit위치에서 가장 가까운 네비게이션 Location 반환
			FNavLocation NavLocation;
			bool bProjected = Nav->ProjectPointToNavigation(TargetLocation, NavLocation);
			if (bProjected)
			{
				TargetLocation = NavLocation.Location + HalfCapsuleVector;
			}
		}
		
		if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
		{
			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(TeleportEffect, CurrentEventData.EventMagnitude);
			if (SourceASC && EffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

				FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
				CueContextHandle.AddInstigator(SourceASC->GetAvatarActor(), SourceASC->GetAvatarActor());
				CueContextHandle.AddHitResult(HitResult);
				
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;
				SourceASC->ExecuteGameplayCue(UNTAG_GAMEPLAYCUE_CHARACTER_TELEPORTEFFECT, CueParam);

				if (APawn* Pawn = Cast<APawn>(AvatarActor))
				{
					Pawn->GetController()->StopMovement();
				}
					
				ServerRPCTeleportToLocation(TargetLocation, CueParam);
				StartCoolDown();
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Location is null!"));
		}

		bool bReplicatedEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
	}
}

void UUNGA_Teleport::OnInterruptedCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UUNGA_Teleport::OnCancelCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	//bool bReplicatedEndAbility = false;
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

// Decal을 이용한 범위 표시
void UUNGA_Teleport::ActivateDecal()
{
	SourceInterface->ActivateDecal_Implementation(DecalStruct);
}

void UUNGA_Teleport::EndDecal()
{
	SourceInterface->EndDecal_Implementation();
}

bool UUNGA_Teleport::ServerRPCTeleportToLocation_Validate(FVector NewLocation, FGameplayCueParameters Params)
{
	// [BUG] 현재 크래시 이슈 때문에 true반환
	return true;
	float ParamLocationDistance = FVector::Dist(Params.Location, AvatarActor->GetActorLocation());
	float NewLocationDistance = FVector::Dist(NewLocation, AvatarActor->GetActorLocation());
	float MaxDistance = DecalStruct.GetScale().Z;

	// Location의 값이 없을 때, 데이터가 없다고 판단하여 false리턴
	if (Params.Location.IsNearlyZero() || NewLocation.IsNearlyZero())
	{
		UE_LOG(LogTemp, Log, TEXT("[ParamLocation | NewLocation] is not valid"));
		return false;
	}

	// Location의 값이 최대치를 넘었을 때(계산식이 들어간 이후) false리턴 (핵 방지)
	if (ParamLocationDistance > MaxDistance || NewLocationDistance > MaxDistance)
	{
		UE_LOG(LogTemp, Log, TEXT("NewLocation is not valid"));
		return false;
	}

	return true;
}

void UUNGA_Teleport::ServerRPCTeleportToLocation_Implementation(FVector NewLocation, FGameplayCueParameters Params)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
	#pragma region SourceASC NullCheck & return
	if (!SourceASC)
	{
		UE_LOG(LogTemp, Log, TEXT("SourceASC is Null!"));
		return;
	}
#pragma endregion

	FVector PlayerLocation = AvatarActor->GetActorLocation();
	Params.Location = SourceASC->GetAvatarActor()->GetActorLocation();
	SourceASC->ExecuteGameplayCue(UNTAG_GAMEPLAYCUE_CHARACTER_TELEPORTEFFECT, Params);

	AvatarActor->TeleportTo(NewLocation, (NewLocation - PlayerLocation).Rotation(), false, true);
}

void UUNGA_Teleport::StartCoolDown_Implementation()
{
	CommitAbilityCooldown(FGameplayAbilitySpecHandle(), CurrentActorInfo, GetCurrentActivationInfoRef(), false);
}
