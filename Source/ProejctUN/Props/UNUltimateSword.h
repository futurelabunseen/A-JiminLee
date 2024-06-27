// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "UNUltimateSword.generated.h"

UCLASS()
class PROEJCTUN_API AUNUltimateSword : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUNUltimateSword();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories = GameplayCue))
	FGameplayTag GameplayCueTag;

	void ApplyEffectToTarget(AActor* Target);
	void InvokeGameplayCue(AActor* Target);

	UPROPERTY(Replicated)
	USkeletalMesh* WeaponMesh;

public:
	
	UFUNCTION()
	void SetMeshToWeaponMesh(USkeletalMesh* NewMesh);
};
