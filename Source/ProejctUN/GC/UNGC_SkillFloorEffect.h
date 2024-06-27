// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "UNGC_SkillFloorEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNGC_SkillFloorEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UUNGC_SkillFloorEffect();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<class UParticleSystem> ParticleSystem;
};
