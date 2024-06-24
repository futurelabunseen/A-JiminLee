// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/UNTA_Trace.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/UNCollision.h"
#include "Engine/DamageEvents.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Attribute/UNCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "DrawDebugHelpers.h"

// �ҽ����� ĳ��
void AUNTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

// Ÿ�� Ž�� �� ��������Ʈ����
void AUNTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

// Ÿ�� Ž��
FGameplayAbilityTargetDataHandle AUNTA_Trace::MakeTargetData() const
{
	ACharacter* Character = Cast<ACharacter>(SourceActor);
	if (!Character)
	{
		return FGameplayAbilityTargetDataHandle();
	}

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

	// Params�� ���� ������ ���� �� Ž�� ����
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UUNAT_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	const float NewRange = AttributeSet->GetAttackRange() / 2;
	FRotator CharacterRotation = Character->GetActorRotation();
	FQuat Rotation = FQuat(CharacterRotation + (FRotator(0.f, 0.f, 90.f))); //FVector(0.f, 1.f, 0.f)
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, Rotation, CCHANNEL_UNACTION, FCollisionShape::MakeCapsule(AttackRadius, NewRange), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.UniqueId = 0;
	if (HitDetected)
	{
		//������ TakeDamage��� GE�� ��� ��. ���ʿ��ϴٰ� �Ǵ� �� �ּ� ���� ����
		//FDamageEvent DamageEvent;
		//OutHitResult.GetActor()->TakeDamage(5.f, DamageEvent, SourceActor->GetInstigatorController(), SourceActor);

		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

// ���� �����

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = NewRange;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, Rotation, DrawColor, false, 5.f);
	}

#endif

	return DataHandle;
}
