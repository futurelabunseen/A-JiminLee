// Fill out your copyright notice in the Description page of Project Settings.


#include "UNTA_TraceLocation.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Player/UNPlayerController.h"

void AUNTA_TraceLocation::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AUNTA_TraceLocation::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AUNTA_TraceLocation::MakeTargetData() const
{
	FHitResult OutHitResult;

	APlayerController* Controller = Cast<APlayerController>(SourceActor->GetInstigatorController());
	if (Controller)
	{
		Controller->GetHitResultUnderCursor(ECC_Visibility, true, OutHitResult);
	}

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
	DataHandle.Add(TargetData);
	return DataHandle;
}
