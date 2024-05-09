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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);
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
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, DefaultAttackRange);
	ATTRIBUTE_ACCESSORS(UUNCharacterAttributeSet, DefaultAttackRate);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FOutOfHealthDelegate OnOutOfHealth;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_AttackRange, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxAttackRange, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_AttackRadius, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxAttackRadius, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_AttackRate, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxAttackRate, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Health, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxHealth, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Damage, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_DefaultAttackRange, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DefaultAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_DefaultAttackRate,Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DefaultAttackRate;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldDamage);

	UFUNCTION()
	void OnRep_AttackRange(const FGameplayAttributeData& OldAttackRange);

	UFUNCTION()
	void OnRep_MaxAttackRange(const FGameplayAttributeData& OldMaxAttackRange);

	UFUNCTION()
	void OnRep_AttackRadius(const FGameplayAttributeData& OldAttackRadius);

	UFUNCTION()
	void OnRep_MaxAttackRadius(const FGameplayAttributeData& OldMaxAttackRadius);

	UFUNCTION()
	void OnRep_AttackRate(const FGameplayAttributeData& OldAttackRate);

	UFUNCTION()
	void OnRep_MaxAttackRate(const FGameplayAttributeData& OldMaxAttackRate);

	UFUNCTION()
	void OnRep_DefaultAttackRange(const FGameplayAttributeData& OldDefaultAttackRange);

	UFUNCTION()
	void OnRep_DefaultAttackRate(const FGameplayAttributeData& OldDefaultAttackRate);

	bool bOutOfHealth = false;

	friend class UUNGE_AttackDamage;
};
