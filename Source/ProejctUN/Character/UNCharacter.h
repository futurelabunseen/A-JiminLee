// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Struct/DecalStruct.h"
#include "UNCharacter.generated.h"

UCLASS()
class PROEJCTUN_API AUNCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUNCharacter();

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* Decal;

// Dead
protected:
	//virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCPlayAnimation(AUNCharacter* Character);

	UFUNCTION()
	virtual void OnOutOfHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UUNGASWidgetComponent> HpBar;

	UPROPERTY(EditAnywhere)
	float DeadEventDelayTime;

	void SetDead();
	void PlayDeadAnimaition();

// Decal

private:
	UPROPERTY()
	FDecalStruct CurrentActiveDecalData;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateDecal(FDecalStruct DecalStruct);

	UFUNCTION(BlueprintCallable)
	void EndDecal();

	UFUNCTION(BlueprintCallable)
	FDecalStruct GetCurrentActiveDecalData() { return CurrentActiveDecalData; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentActiveDecalData(FDecalStruct NewDecalData) { CurrentActiveDecalData = NewDecalData; }

	UFUNCTION(BlueprintCallable)
	void ClearCurrentActiveDecalData() { CurrentActiveDecalData = FDecalStruct(); }
};
