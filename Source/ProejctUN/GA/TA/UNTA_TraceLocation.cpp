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
	FHitResult OutHitResult;
	FNavLocation NavLocation;

	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(SourceActor);
	if (PlayerCharacter)
	{
		APlayerController* Controller = Cast<APlayerController>(SourceActor->GetInstigatorController());
		if (Controller)
		{
			Controller->GetHitResultUnderCursor(ECC_Visibility, true, OutHitResult);

			float SkillRangeScale = PlayerCharacter->GetCurrentActiveDecalData().GetScale().Z;
			float HitDistance = (OutHitResult.Location - PlayerCharacter->GetActorLocation()).Size();

			if (HitDistance > SkillRangeScale)
			{
				FVector SkillRangeEnd = PlayerCharacter->GetActorLocation() + (OutHitResult.Location - PlayerCharacter->GetActorLocation()).GetSafeNormal() * SkillRangeScale;
				OutHitResult.Location = SkillRangeEnd;
			}

			UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			if (Nav)
			{
				// Hit��ġ���� ���� ����� �׺���̼� Location ��ȯ
				bool bProjected = Nav->ProjectPointToNavigation(OutHitResult.Location, NavLocation);
				OutHitResult.Location = NavLocation.Location;
			}
		}
	}

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
	DataHandle.Add(TargetData);
	return DataHandle;
}
