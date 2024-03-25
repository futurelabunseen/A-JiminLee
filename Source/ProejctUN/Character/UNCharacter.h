// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UNCharacter.generated.h"

UCLASS()
class PROEJCTUN_API AUNCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUNCharacter();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//FORCEINLINE virtual class UAnimMontage* GetComboActionMontage() const { return ComboActionMontage; }
	//FORCEINLINE class UUNComboActionData* GetComboActionData() const { return ComboActionData; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	//TObjectPtr<class UAnimMontage> ComboActionMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UUNComboActionData> ComboActionData;

//Dead
protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimaition();

	UPROPERTY(EditAnywhere)
	float DeadEventDelayTime;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UUNGASWidgetComponent> HpBar;
};
