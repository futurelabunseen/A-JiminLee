// Fill out your copyright notice in the Description page of Project Settings.


#include "../Player/UNGASPlayerState.h"
#include "AbilitySystemComponent.h"

AUNGASPlayerState::AUNGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* AUNGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
