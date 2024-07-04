// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UNAbilitySystemComponent.generated.h"

/**
 * 
 */
class UAnimMontage;
class UUNComboActionData;

UCLASS()
class PROEJCTUN_API UUNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	
protected:
	TObjectPtr<AGameplayAbilityTargetActor> CurrentActiveTargetActor;

	TObjectPtr<UGameplayAbility> CurrentActiveAbility;

public:
	TObjectPtr<AGameplayAbilityTargetActor> GetCurrentActiveTargetActor() { return CurrentActiveTargetActor; }
	void SetCurrentActiveTargetActor(TObjectPtr<AGameplayAbilityTargetActor> NewCurrentActiveTargetActor) { CurrentActiveTargetActor = NewCurrentActiveTargetActor; }

	TObjectPtr<UGameplayAbility> GetCurrentActiveAbility() { return CurrentActiveAbility; }
	void SetCurrentActiveAbility(TObjectPtr<UGameplayAbility> NewCurrentActiveAbility) { CurrentActiveAbility = NewCurrentActiveAbility; }

	UPROPERTY()
	uint32 bAbilityAlreadyActivate;

	UFUNCTION()
	void ActivateGameplayAbilityWithClass(TSubclassOf<UGameplayAbility> ability);


//////////////////////////////////////////////////////////////
///////////////////////// UNGA_Attack ////////////////////////
protected:
	UPROPERTY()
	TObjectPtr<UAnimMontage> ComboActionMontage;

	UPROPERTY()
	TObjectPtr<UUNComboActionData> ComboActionData;

public:
	FORCEINLINE virtual UAnimMontage* GetComboActionMontage() const { return ComboActionMontage; }
	FORCEINLINE virtual void SetComboActionMontage(UAnimMontage* NewMontage) { ComboActionMontage = NewMontage; }

	FORCEINLINE virtual UUNComboActionData* GetComboActionData() const { return ComboActionData; }
	FORCEINLINE virtual void SetComboActionData(UUNComboActionData* NewComboActionData) { ComboActionData = NewComboActionData; }
////////////////////////UNGA_Attack End///////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///////////////////////// UNGA_Skill /////////////////////////
protected:
	UPROPERTY()
	TObjectPtr<UAnimMontage> SkillActionMontage;

public:
	FORCEINLINE virtual UAnimMontage* GetSkillActionMontage() const { return SkillActionMontage; }
	FORCEINLINE virtual void SetSkillActionMontage(UAnimMontage* NewMontage) { SkillActionMontage = NewMontage; }
/////////////////////// UNGA_Skill End////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/////////////////////// UNGA_Ultimate ////////////////////////

protected:
	UPROPERTY()
	FVector UltimateLocation;

	UPROPERTY()
	TObjectPtr<class USkeletalMesh> UltimateMesh;
public:
	UFUNCTION()
	void SetUltimateLocation(FVector Location) { UltimateLocation = Location; }

	UFUNCTION()
	FVector GetUltimateLocation() { return UltimateLocation; }

	UFUNCTION()
	void SetUltimateMesh(USkeletalMesh* Mesh) { UltimateMesh = Mesh; }

	UFUNCTION()
	USkeletalMesh* GetUltimateMesh() { return UltimateMesh; }

/////////////////////// UNGA_Ultimate End ////////////////////
//////////////////////////////////////////////////////////////
};
