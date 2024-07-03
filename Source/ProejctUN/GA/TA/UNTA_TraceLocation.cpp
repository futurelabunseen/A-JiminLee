// Fill out your copyright notice in the Description page of Project Settings.


#include "UNTA_TraceLocation.h"
#include "NavigationSystem.h"

//#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Character/UNPlayerCharacter.h"
#include "Player/UNPlayerController.h"

void AUNTA_TraceLocation::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AUNTA_TraceLocation::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

void AUNTA_TraceLocation::CancelTargeting()
{
	Super::CancelTargeting();
}

// 마우스 위치를 지점으로 FHitResult를 생성한뒤 핸들에 데이터넣고 반환
FGameplayAbilityTargetDataHandle AUNTA_TraceLocation::MakeTargetData() const
{
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(SourceActor);
	#pragma region
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerCharacter is Null"));
		return FGameplayAbilityTargetDataHandle();
	}
	#pragma endregion

	APlayerController* Controller = Cast<APlayerController>(SourceActor->GetInstigatorController());
	#pragma region
	if (!Controller)
	{
		UE_LOG(LogTemp, Log, TEXT("Controller is Null"));
		return FGameplayAbilityTargetDataHandle();
	}
	#pragma endregion

	// 마우스 히트 위치 체크
	FHitResult OutHitResult;
	Controller->GetHitResultUnderCursor(ECC_Visibility, true, OutHitResult);

	// 스킬 거리와 히트 위치 비교
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	float SkillRangeScale = PlayerCharacter->GetCurrentActiveDacalData_Implementation().GetScale().Z;
	float HitDistance = (OutHitResult.Location - PlayerLocation).Size();

	// 히트 위치가 스킬 범위보다 크다면 위치 조정
	if (HitDistance > SkillRangeScale)
	{
		FVector Dir = (OutHitResult.Location - PlayerLocation).GetSafeNormal();
		FVector SkillRangeLimit = Dir * SkillRangeScale + PlayerLocation;
		OutHitResult.Location = SkillRangeLimit;
	}

	// 핸들에 위치 데이터 넣고 반환
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* TargetData =
		new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
	DataHandle.Add(TargetData);
	return DataHandle;
}
