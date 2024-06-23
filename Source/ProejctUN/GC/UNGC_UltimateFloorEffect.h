// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "UNGC_UltimateFloorEffect.generated.h"

/**
 * 
 */

class UNiagaraSystem;

UCLASS()
class PROEJCTUN_API UUNGC_UltimateFloorEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UUNGC_UltimateFloorEffect();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<UNiagaraSystem> Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<UNiagaraSystem> Niagara2;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> Sound;
};
