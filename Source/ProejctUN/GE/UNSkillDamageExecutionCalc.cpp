// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/UNSkillDamageExecutionCalc.h"
#include "AbilitySystemComponent.h"
#include "Attribute/UNCharacterAttributeSet.h"
#include "Attribute/UNCharacterSkillAttributeSet.h"

void UUNSkillDamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceASC && TargetASC)
	{
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();

		if (SourceActor && TargetActor)
		{
			const float MaxDamageRange = SourceASC->GetNumericAttributeBase(UUNCharacterSkillAttributeSet::GetSKillRangeAttribute());
			const float MaxDamage = SourceASC->GetNumericAttributeBase(UUNCharacterSkillAttributeSet::GetSKillRateAttribute());

			const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.f, MaxDamageRange);
			const float InvDamageRatio = 1.f - Distance / MaxDamageRange;
			float Damage = InvDamageRatio * MaxDamage;

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UUNCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
		}
	}
}
