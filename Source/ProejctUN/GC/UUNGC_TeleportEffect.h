// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "UUNGC_TeleportEffect.generated.h"

/**
 * 
 */

class UNiagaraSystem;

UCLASS()
class PROEJCTUN_API UUUNGC_TeleportEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UUUNGC_TeleportEffect();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	//TObjectPtr<class UParticleSystem> ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<UNiagaraSystem> Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<USoundBase> Sound;
};

