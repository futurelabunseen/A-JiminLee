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
				// Hit위치에서 가장 가까운 네비게이션 Location 반환
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
