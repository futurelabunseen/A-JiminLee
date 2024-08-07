// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "UNGC_StunEffect.generated.h"

/**
 * 
 */

class UNiagaraSystem;

UCLASS()
class PROEJCTUN_API UUNGC_StunEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UUNGC_StunEffect();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<UNiagaraSystem> Niagara;
};
