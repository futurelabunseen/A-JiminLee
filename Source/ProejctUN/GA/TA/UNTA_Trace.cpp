// Fill out your copyright notice in the Description page of Project Settings.


#include "../../GA/TA/UNTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "../../Physics/UNCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "AbilitySystemComponent.h"
#include "../../Attribute/UNCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

AUNTA_Trace::AUNTA_Trace()
{
}

void AUNTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AUNTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AUNTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	const UUNCharacterAttributeSet* AttributeSet = ASC->GetSet<UUNCharacterAttributeSet>();
	if (!AttributeSet)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	FHitResult OutHitResult;
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();


	FCollisionQueryParams Params(SCENE_QUERY_STAT(UUNAT_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_UNACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		//FDamageEvent DamageEvent;
		//OutHitResult.GetActor()->TakeDamage(5.f, DamageEvent, SourceActor->GetInstigatorController(), SourceActor);

		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.f);
	}

#endif

	return DataHandle;
}
