// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UNGASPlayerState.h"
#include "ASC/UNAbilitySystemComponent.h"
#include "Attribute/UNCharacterAttributeSet.h"
#include "Attribute/UNCharacterSkillAttributeSet.h"

AUNGASPlayerState::AUNGASPlayerState()
{
	ASC = CreateDefaultSubobject<UUNAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UUNCharacterAttributeSet>(TEXT("AttributeSet"));
	SkillAttributeSet = CreateDefaultSubobject<UUNCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));
	ASC->SetIsReplicated(true);
	NetUpdateFrequency = 60.f;
}

UAbilitySystemComponent* AUNGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

UUNCharacterAttributeSet* AUNGASPlayerState::GetAttributeSet()
{
	return AttributeSet;
}
