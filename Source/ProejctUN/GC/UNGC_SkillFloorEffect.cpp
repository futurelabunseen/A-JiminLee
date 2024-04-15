// Fill out your copyright notice in the Description page of Project Settings.


#include "UNGC_SkillFloorEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

UUNGC_SkillFloorEffect::UUNGC_SkillFloorEffect()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FloorEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/OutsideAsset/ParagonGreystone/FX/Particles/Greystone/Abilities/Ultimate/FX/P_Greystone_Ultimate_Slow.P_Greystone_Ultimate_Slow'"));
	if (FloorEffectRef.Object)
	{
		ParticleSystem = FloorEffectRef.Object;
	}
}

bool UUNGC_SkillFloorEffect::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}

	else
	{
		for (const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
		{
			if (TargetActor.Get())
			{
				UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, Parameters.EffectContext.Get()->GetInstigator()->GetActorLocation(), FRotator::ZeroRotator, true);
				//UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
			}
		}
	}

	return false;
}