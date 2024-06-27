// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UNAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNAT_Trace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UUNAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AUNTA_Trace> TargetActorClass);

	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitalizeTargetActor();
	void FinalizeTargetActor();

protected:
	UPROPERTY()
	TSubclassOf<class AUNTA_Trace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AUNTA_Trace> SpawnedTargetActor;

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);
};
