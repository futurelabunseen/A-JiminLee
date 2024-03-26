// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/UNNonPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "../Attribute/UNCharacterAttributeSet.h"

AUNNonPlayerCharacter::AUNNonPlayerCharacter()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UUNCharacterAttributeSet>(TEXT("AttributeSet"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Level = 1.f;
}

UAbilitySystemComponent* AUNNonPlayerCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AUNNonPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
	AttributeSet->OnOutOfHealth.AddDynamic(this, &AUNNonPlayerCharacter::OnOutOfHealth);

	FGameplayEffectContextHandle EffectContectHandle = ASC->MakeEffectContext();
	EffectContectHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContectHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}
