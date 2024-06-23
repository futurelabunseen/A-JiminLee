// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UNCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathDelegate, AUNCharacter*, Character);

UCLASS()
class PROEJCTUN_API AUNCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUNCharacter();

	virtual void PostInitializeComponents() override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnDeathDelegate OnDeath;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

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
};
