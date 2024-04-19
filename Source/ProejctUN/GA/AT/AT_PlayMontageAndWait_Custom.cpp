// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_PlayMontageAndWait_Custom.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "AbilitySystemGlobals.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

UAT_PlayMontageAndWait_Custom::UAT_PlayMontageAndWait_Custom()
{
	bSimulatedTask = true;
}

UAT_PlayMontageAndWait_Custom* UAT_PlayMontageAndWait_Custom::CreatePlayMontageAndWaitProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds, bool bAllowInterruptAfterBlendOut)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UAT_PlayMontageAndWait_Custom* MyObj = NewAbilityTask<UAT_PlayMontageAndWait_Custom>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->bAllowInterruptAfterBlendOut = bAllowInterruptAfterBlendOut;
	MyObj->StartTimeSeconds = StartTimeSeconds;

	return MyObj;
}

void UAT_PlayMontageAndWait_Custom::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	UGameplayTask::InitSimulatedTask(InGameplayTasksComponent);

	SetAbilitySystemComponent(Cast<UAbilitySystemComponent>(TasksComponent.Get()));
}

void UAT_PlayMontageAndWait_Custom::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	//DOREPLIFETIME(UAT_PlayMontageAndWait_Custom, MontageToPlay);
	//DOREPLIFETIME(UAT_PlayMontageAndWait_Custom, Rate);
	//DOREPLIFETIME(UAT_PlayMontageAndWait_Custom, StartSection);
	//DOREPLIFETIME(UAT_PlayMontageAndWait_Custom, AnimRootMotionTranslationScale);
	//DOREPLIFETIME(UAT_PlayMontageAndWait_Custom, bStopWhenAbilityEnds);
	//DOREPLIFETIME(UAT_PlayMontageAndWait_Custom, bAllowInterruptAfterBlendOut);
	//DOREPLIFETIME(UAT_PlayMontageAndWait_Custom, StartTimeSeconds);

}