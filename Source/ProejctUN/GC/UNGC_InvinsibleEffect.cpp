// Fill out your copyright notice in the Description page of Project Settings.


#include "UNGC_InvinsibleEffect.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

UUNGC_InvinsibleEffect::UUNGC_InvinsibleEffect()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> InvinsibleEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/OutsideAsset/sA_PickupSet_1/Fx/NiagaraSystems/NS_Shield_2.NS_Shield_2'"));
	if (InvinsibleEffectRef.Object)
	{
		Niagara = InvinsibleEffectRef.Object;
	}
}

bool UUNGC_InvinsibleEffect::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	AActor* Instigator = Parameters.EffectContext.GetInstigator();
	if (Instigator)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Niagara, Instigator->GetActorLocation(), Instigator->K2_GetActorRotation());
	}
	

	return false;
}
