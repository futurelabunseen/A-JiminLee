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

// ���콺 ��ġ�� �������� FHitResult�� �����ѵ� �ڵ鿡 �����ͳְ� ��ȯ
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

	// ���콺 ��Ʈ ��ġ üũ
	FHitResult OutHitResult;
	Controller->GetHitResultUnderCursor(ECC_Visibility, true, OutHitResult);

	// ��ų �Ÿ��� ��Ʈ ��ġ ��
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	float SkillRangeScale = PlayerCharacter->GetCurrentActiveDacalData_Implementation().GetScale().Z;
	float HitDistance = (OutHitResult.Location - PlayerLocation).Size();

	// ��Ʈ ��ġ�� ��ų �������� ũ�ٸ� ��ġ ����
	if (HitDistance > SkillRangeScale)
	{
		FVector Dir = (OutHitResult.Location - PlayerLocation).GetSafeNormal();
		FVector SkillRangeLimit = Dir * SkillRangeScale + PlayerLocation;
		OutHitResult.Location = SkillRangeLimit;
	}

	// �ڵ鿡 ��ġ ������ �ְ� ��ȯ
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* TargetData =
		new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
	DataHandle.Add(TargetData);
	return DataHandle;
}
