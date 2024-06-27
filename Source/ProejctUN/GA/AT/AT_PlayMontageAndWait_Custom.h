// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimInstance.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AT_PlayMontageAndWait_Custom.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class PROEJCTUN_API UAT_PlayMontageAndWait_Custom : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()

public:

	//ATTRIBUTE_ACCESSORS(UAT_PlayMontageAndWait_Custom, MontageToPlay);
	//ATTRIBUTE_ACCESSORS(UAT_PlayMontageAndWait_Custom, Rate);
	//ATTRIBUTE_ACCESSORS(UAT_PlayMontageAndWait_Custom, StartSection);
	//ATTRIBUTE_ACCESSORS(UAT_PlayMontageAndWait_Custom, AnimRootMotionTranslationScale);
	//ATTRIBUTE_ACCESSORS(UAT_PlayMontageAndWait_Custom, bStopWhenAbilityEnds);
	//ATTRIBUTE_ACCESSORS(UAT_PlayMontageAndWait_Custom, bAllowInterruptAfterBlendOut);
	//ATTRIBUTE_ACCESSORS(UAT_PlayMontageAndWait_Custom, StartTimeSeconds);

	//UPROPERTY(Replicated, Meta = (AllowPrivateAccess = true))
	//UAnimMontage* MontageToPlay;
	//UPROPERTY(Replicated, Meta = (AllowPrivateAccess = true))
	//float Rate;
	//UPROPERTY(Replicated, Meta = (AllowPrivateAccess = true))
	//FName StartSection;
	//UPROPERTY(Replicated, Meta = (AllowPrivateAccess = true))
	//float AnimRootMotionTranslationScale;
	//UPROPERTY(Replicated, Meta = (AllowPrivateAccess = true))
	//bool bStopWhenAbilityEnds;
	//UPROPERTY(Replicated, Meta = (AllowPrivateAccess = true))
	//float StartTimeSeconds;
	//UPROPERTY(Replicated, Meta = (AllowPrivateAccess = true))
	//bool bAllowInterruptAfterBlendOut;

	UAT_PlayMontageAndWait_Custom();

	static UAT_PlayMontageAndWait_Custom* CreatePlayMontageAndWaitProxy(UGameplayAbility* OwningAbility,
		FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f, bool bAllowInterruptAfterBlendOut = false);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;
};