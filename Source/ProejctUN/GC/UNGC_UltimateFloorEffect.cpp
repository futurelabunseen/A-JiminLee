// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/UNGC_UltimateFloorEffect.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/UNPlayerCharacter.h"
#include "NiagaraSystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ASC/UNAbilitySystemComponent.h"

UUNGC_UltimateFloorEffect::UUNGC_UltimateFloorEffect()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> UltimateFloorEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/RPGEffects/ParticlesNiagara/Warrior/HeavyImpact/NS_Warrior_HeavyImpact.NS_Warrior_HeavyImpact'"));
	if (UltimateFloorEffectRef.Object)
	{
		Niagara = UltimateFloorEffectRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> UltimateFloorEffectRef2(TEXT("/Script/Niagara.NiagaraSystem'/Game/RPGEffects/ParticlesNiagara/Warrior/EarthCrack/NS_Warrior_EarthCrack.NS_Warrior_EarthCrack'"));
	if (UltimateFloorEffectRef2.Object)
	{
		Niagara2 = UltimateFloorEffectRef2.Object;
	}
}

bool UUNGC_UltimateFloorEffect::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	//if (AActor* Instigator = Parameters.GetInstigator())
	//{
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, Niagara, Instigator->GetActorLocation(), FRotator::ZeroRotator);
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, Niagara2, Instigator->GetActorLocation(), FRotator::ZeroRotator);
	//}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, Niagara, Parameters.Location, FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(Target, Niagara2, Parameters.Location, FRotator::ZeroRotator);
	UGameplayStatics::PlaySoundAtLocation(this, Sound, Parameters.Location);

	return false;
}
