// Fill out your copyright notice in the Description page of Project Settings.


#include "UNAbilitySystemComponent.h"

void UUNAbilitySystemComponent::ActivateGameplayAbilityWithClass(TSubclassOf<UGameplayAbility> ability)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(ability);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			AbilitySpecInputPressed(*Spec);
		}
		else
		{
			TryActivateAbility(Spec->Handle);
		}
	}
}
