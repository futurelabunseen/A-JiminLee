// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/UNTA_SphereMultiTrace.h"
#include "Abilities/GameplayAbility.h"
#include "Attribute/UNCharacterSkillAttributeSet.h"

#include "GameFramework/Character.h"
#include "Physics/UNCollision.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "DrawDebugHelpers.h"
#include "ProejctUN.h"

// 타겟 지정 후 핸들 리턴
FGameplayAbilityTargetDataHandle AUNTA_SphereMultiTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		UN_LOG(LogUNNetwork, Log, TEXT("Can't find ASC!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UUNCharacterSkillAttributeSet* SkillAttribute = ASC->GetSet<UUNCharacterSkillAttributeSet>();
	if (!SkillAttribute)
	{
		UN_LOG(LogUNNetwork, Log, TEXT("Can't find Attribute!"));
		return FGameplayAbilityTargetDataHandle();
	}

	TArray<FOverlapResult> Overlaps;
	const float SkillRadius = SkillAttribute->GetSKillRange();

	// Params로 관련 데이터 설정 후 OverlapMultiByChannel을 통해 Overlaps에 피격 데이터 저장
	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AUNTA_SphereMultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_UNACTION, FCollisionShape::MakeSphere(SkillRadius), Params);

	// 데이터들을 선별 후 약포인터 배열에 추가
	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();

		if (HitActor && !HitActors.Contains(HitActor) && Cast<ACharacter>(HitActor))
		{
			HitActors.Add(HitActor);
			//UE_LOG(LogTemp, Log, TEXT("%s"), *HitActor->GetName());
		}
	}
	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	ActorsData->SetActors(HitActors);

	FGameplayAbilityTargetDataHandle TargetDataHandle(ActorsData);
	TargetDataHandle.UniqueId = 1;

// 범위 디버그
//
//#if ENABLE_DRAW_DEBUG
//
//	if (bShowDebug)
//	{
//		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
//		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.f);
//	}
//#endif

	return TargetDataHandle;
	//return FGameplayAbilityTargetDataHandle(ActorsData);
}
