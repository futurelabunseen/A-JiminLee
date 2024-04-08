// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "UNTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

//Debug

public:
	void SetShowDebug(bool InshowDebug) { bShowDebug = InshowDebug; }

protected:
	bool bShowDebug = false;
};
