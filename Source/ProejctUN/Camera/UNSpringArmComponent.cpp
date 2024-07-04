// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/UNSpringArmComponent.h"

UUNSpringArmComponent::UUNSpringArmComponent()
{
}

void UUNSpringArmComponent::UpdateSpringArmLength(float Start, float End, float Time, float Frame)
{
	SpringArmShortLength = Start;
	SpringArmLongLength = End;
	SpringArmStartTime = 0.f;
	SpringArmMoveTime = Time;

	GetWorld()->GetTimerManager().SetTimer(SpringArmUpdateTimerHandle, [&]()
		{
			SpringArmStartTime += 0.01f;

			float Alpha = FMath::Clamp(SpringArmStartTime / SpringArmMoveTime, 0.0f, 1.0f);
			float NewArmLength = FMath::Lerp(SpringArmShortLength, SpringArmLongLength, Alpha);
			this->TargetArmLength = NewArmLength;

			if (Alpha >= 1.0f)
			{
				SpringArmStartTime = 0.f;
				GetWorld()->GetTimerManager().ClearTimer(SpringArmUpdateTimerHandle);
			}
		}, Frame, true);
}

void UUNSpringArmComponent::ReturnSpringArmLength()
{
	TargetArmLength = DefaultSpringArmLength;
}

void UUNSpringArmComponent::ClearSpringArmTimerHandle()
{
	GetWorld()->GetTimerManager().ClearTimer(SpringArmUpdateTimerHandle);
}
