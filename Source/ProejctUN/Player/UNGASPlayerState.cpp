// Fill out your copyright notice in the Description page of Project Settings.


#include "../Player/UNGASPlayerState.h"
#include "AbilitySystemComponent.h"

AUNGASPlayerState::AUNGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* AUNGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
