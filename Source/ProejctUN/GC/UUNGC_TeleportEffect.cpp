// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/UUNGC_TeleportEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

UUUNGC_TeleportEffect::UUUNGC_TeleportEffect()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FloorEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/OutsideAsset/ParagonGreystone/FX/Particles/Greystone/Abilities/Ultimate/FX/P_Greystone_Ultimate_Slow.P_Greystone_Ultimate_Slow'"));
	if (FloorEffectRef.Object)
	{
		ParticleSystem = FloorEffectRef.Object;
	}
}

bool UUUNGC_TeleportEffect::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const AActor* SourceASC = Parameters.EffectContext.GetInstigator();
	if (SourceASC)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, SourceASC->GetActorLocation(), FRotator::ZeroRotator, true);
	}

	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, HitResult->Location, FRotator::ZeroRotator, true);
	}

	//else
	//{
	//	for (const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
	//	{
	//		if (TargetActor.Get())
	//		{
	//			UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, Parameters.EffectContext.Get()->GetInstigator()->GetActorLocation(), FRotator::ZeroRotator, true);
	//			//UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
	//		}
	//	}
	//}

	return false;
}
