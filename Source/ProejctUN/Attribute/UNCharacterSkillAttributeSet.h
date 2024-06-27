// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UNCharacterSkillAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROEJCTUN_API UUNCharacterSkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UUNCharacterSkillAttributeSet();

	ATTRIBUTE_ACCESSORS(UUNCharacterSkillAttributeSet, SKillRange);
	ATTRIBUTE_ACCESSORS(UUNCharacterSkillAttributeSet, MaxSkillRange);
	ATTRIBUTE_ACCESSORS(UUNCharacterSkillAttributeSet, SKillRate);
	ATTRIBUTE_ACCESSORS(UUNCharacterSkillAttributeSet, MaxSkillRate);
	ATTRIBUTE_ACCESSORS(UUNCharacterSkillAttributeSet, SKillEnergy);
	ATTRIBUTE_ACCESSORS(UUNCharacterSkillAttributeSet, MaxSkillEnergy);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SKillRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SKillRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SKillEnergy;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillEnergy;

};
