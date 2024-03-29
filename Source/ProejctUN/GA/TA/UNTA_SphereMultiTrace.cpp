// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/UNTA_SphereMultiTrace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Physics/UNCollision.h"
#include "DrawDebugHelpers.h"
#include "Attribute/UNCharacterSkillAttributeSet.h"

FGameplayAbilityTargetDataHandle AUNTA_SphereMultiTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		UE_LOG(LogTemp, Log, TEXT("ASC Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UUNCharacterSkillAttributeSet* SkillAttribute = ASC->GetSet<UUNCharacterSkillAttributeSet>();
	if (!SkillAttribute)
	{
		UE_LOG(LogTemp, Log, TEXT("SkillAttribute Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	TArray<FOverlapResult> Overlaps;
	const float SkillRadius = SkillAttribute->GetSKillRange();

	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AUNTA_SphereMultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_UNACTION, FCollisionShape::MakeSphere(SkillRadius), Params);

	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();

		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
			UE_LOG(LogTemp, Log, TEXT("%s"), *HitActor->GetName());
		}
	}

	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	ActorsData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.f);
	}
#endif

	return FGameplayAbilityTargetDataHandle(ActorsData);
}
