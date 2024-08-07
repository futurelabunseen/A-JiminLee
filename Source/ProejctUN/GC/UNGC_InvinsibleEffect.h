// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "UNGC_InvinsibleEffect.generated.h"

/**
 * 
 */

class UNiagaraSystem;
class USoundBase;

UCLASS()
class PROEJCTUN_API UUNGC_InvinsibleEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UUNGC_InvinsibleEffect();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<UNiagaraSystem> Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<UNiagaraSystem> FloorNiagara;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<USoundBase> Sound;

};
