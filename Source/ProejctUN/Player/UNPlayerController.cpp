// Fill out your copyright notice in the Description page of Project Settings.


#include "UNPlayerController.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AUNPlayerController::AUNPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	//CachedDestination = FVector::ZeroVector;
	//FollowTime = 0.f;
	//ShortPressThreshold = 0.3f;

	//static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Character.IMC_Character'"));
	//if (nullptr != InputMappingContextRef.Object)
	//{
	//	DefaultMappingContext = InputMappingContextRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Move.IA_Move'"));
	//if (nullptr != InputActionMoveRef.Object)
	//{
	//	SetDestinationClickAction = InputActionMoveRef.Object;
	//}
}

void AUNPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	//{
	//	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//}
}
//
//void AUNPlayerController::SetupInputComponent()
//{
//	Super::SetupInputComponent();
//
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
//	{
//		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AUNPlayerController::OnInputStarted);
//		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AUNPlayerController::OnSetDestinationTriggered);
//		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AUNPlayerController::OnSetDestinationReleased);
//		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AUNPlayerController::OnSetDestinationReleased);
//	}
//}

//void AUNPlayerController::OnInputStarted()
//{
//	StopMovement();
//}
//
//void AUNPlayerController::OnSetDestinationTriggered()
//{
//	FollowTime += GetWorld()->GetDeltaSeconds();
//
//	FHitResult Hit;
//	bool bHitSuccessful = false;
//	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
//
//	if (bHitSuccessful)
//	{
//		CachedDestination = Hit.Location;
//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("%f"), CachedDestination.X));
//	}
//
//	APawn* ControlledPawn = GetPawn();
//	if (ControlledPawn != nullptr)
//	{
//		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
//		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("%f"), FollowTime));
//	}
//}
//
//void AUNPlayerController::OnSetDestinationReleased()
//{
//	if (FollowTime <= ShortPressThreshold)
//	{
//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
//	}
//
//	FollowTime = 0.f;
//
//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Released!")));
//}
