// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_GASAttackHitCheck::UAnimNotify_GASAttackHitCheck()
{
	ComboAttackLevel = 1.f;
}

FString UAnimNotify_GASAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASAttackHitCheck");
}

// 함수가 실행되면 게임플레이태그를 통해 이벤트를 발생
void UAnimNotify_GASAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			//PayloadData를 통해 이벤트 관련 데이터를 전달
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = ComboAttackLevel;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
