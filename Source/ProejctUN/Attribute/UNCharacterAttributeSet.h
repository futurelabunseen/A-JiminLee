// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UNCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//ATTRIBUTE_ACCESSORS(UMyHealthSet, Health)
/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
		
public:
	UUNCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	//virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	friend class UUNGE_AttackDamage;
};
