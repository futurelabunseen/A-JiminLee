// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/UNGC_StunEffect.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/UNPlayerCharacter.h"
#include "NiagaraSystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ASC/UNAbilitySystemComponent.h"

UUNGC_StunEffect::UUNGC_StunEffect()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> StunEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/RPGEffects/ParticlesNiagara/StatusEffects/Stun/NS_Status_Stun.NS_Status_Stun'"));
	if (StunEffectRef.Object)
	{
		Niagara = StunEffectRef.Object;
	}
}

bool UUNGC_StunEffect::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	//AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(Parameters.EffectContext.GetInstigator());
	FGameplayEffectContextHandle Handle = Parameters.EffectContext;

	for (const auto& target : Handle.GetActors())
	{
		AUNPlayerCharacter* PlayerCharacter = Cast<AUNPlayerCharacter>(target.Get());
		if (PlayerCharacter)
		{
			PlayerCharacter->UpdateHeadNiagara(Niagara);
		}
	}

	return false;
}
