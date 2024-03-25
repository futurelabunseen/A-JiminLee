// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/UNNonPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "../Attribute/UNCharacterAttributeSet.h"

AUNNonPlayerCharacter::AUNNonPlayerCharacter()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UUNCharacterAttributeSet>(TEXT("AttributeSet"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

UAbilitySystemComponent* AUNNonPlayerCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AUNNonPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
}
