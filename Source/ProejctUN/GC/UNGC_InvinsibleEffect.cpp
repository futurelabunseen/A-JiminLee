// Fill out your copyright notice in the Description page of Project Settings.


#include "UNGC_InvinsibleEffect.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/UNPlayerCharacter.h"
#include "NiagaraSystem.h"

UUNGC_InvinsibleEffect::UUNGC_InvinsibleEffect()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> InvinsibleEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/OutsideAsset/sA_PickupSet_1/Fx/NiagaraSystems/NS_Shield_2.NS_Shield_2'"));
	if (InvinsibleEffectRef.Object)
	{
		Niagara = InvinsibleEffectRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> InvinsibleFloorEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/OutsideAsset/sA_PickupSet_1/Fx/NiagaraSystems/NS_Shield.NS_Shield'"));
	if (InvinsibleFloorEffectRef.Object)
	{
		FloorNiagara = InvinsibleFloorEffectRef.Object;
	}
}

bool UUNGC_InvinsibleEffect::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(Parameters.EffectContext.GetInstigator());
	
	if (PlayerCharacter)
	{
		PlayerCharacter->UpdateNiagara(Niagara);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FloorNiagara , PlayerCharacter->GetActorLocation(), FRotator::ZeroRotator, FVector(5.f, 5.f, 5.f));
	}
	

	return false;
}
