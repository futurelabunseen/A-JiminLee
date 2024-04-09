// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UNGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/UNCharacterAttributeSet.h"
#include "Attribute/UNCharacterSkillAttributeSet.h"

AUNGASPlayerState::AUNGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UUNCharacterAttributeSet>(TEXT("AttributeSet"));
	SkillAttributeSet = CreateDefaultSubobject<UUNCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));
	ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* AUNGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
