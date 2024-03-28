// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TA/UNTA_Trace.h"
#include "UNTA_SphereMultiTrace.generated.h"

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNTA_SphereMultiTrace : public AUNTA_Trace
{
	GENERATED_BODY()
	
protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
