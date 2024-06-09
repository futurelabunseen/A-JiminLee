// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/UUNGC_TeleportEffect.h"
//#include "Kismet/GameplayStatics.h"
//#include "Particles/ParticleSystem.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/UNPlayerCharacter.h"
#include "NiagaraSystem.h"

UUUNGC_TeleportEffect::UUUNGC_TeleportEffect()
{
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> FloorEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/OutsideAsset/ParagonGreystone/FX/Particles/Greystone/Abilities/Ultimate/FX/P_Greystone_Ultimate_Slow.P_Greystone_Ultimate_Slow'"));
	//if (FloorEffectRef.Object)
	//{
	//	ParticleSystem = FloorEffectRef.Object;
	//}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TeleportEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/RPGEffects/ParticlesNiagara/StatusEffects/TeleportBurst/NS_Status_Teleport_Burst.NS_Status_Teleport_Burst'"));
	if (TeleportEffectRef.Object)
	{
		Niagara = TeleportEffectRef.Object;
	}
}

bool UUUNGC_TeleportEffect::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, Niagara, SourceASC->GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, Niagara, Parameters.Location, FRotator::ZeroRotator);
	//UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, SourceASC->GetActorLocation(), FRotator::ZeroRotator, true);

	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, Niagara, HitResult->Location, FRotator::ZeroRotator);
		//UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, HitResult->Location, FRotator::ZeroRotator, true);
	}
	return false;
}