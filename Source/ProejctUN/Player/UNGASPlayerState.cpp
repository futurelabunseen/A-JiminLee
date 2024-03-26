// Fill out your copyright notice in the Description page of Project Settings.


#include "../Player/UNGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "../Attribute/UNCharacterAttributeSet.h"

AUNGASPlayerState::AUNGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UUNCharacterAttributeSet>(TEXT("AttributeSet"));
	ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* AUNGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
