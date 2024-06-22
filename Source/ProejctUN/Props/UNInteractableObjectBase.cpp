// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNInteractableObjectBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player/UNPlayerController.h"

AUNInteractableObjectBase::AUNInteractableObjectBase()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(BoxCollision);
	SkeletalMesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxCollision);
	Mesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
}

void AUNInteractableObjectBase::BeginPlay()
{
	Super::BeginPlay();

	if (Mesh)
	{
		Mesh->OnBeginCursorOver.AddDynamic(this, &AUNInteractableObjectBase::SendBeginDataToController);
		Mesh->OnEndCursorOver.AddDynamic(this, &AUNInteractableObjectBase::SendEndDataToController);
	}

	if (SkeletalMesh)
	{
		SkeletalMesh->OnBeginCursorOver.AddDynamic(this, &AUNInteractableObjectBase::SendBeginDataToController);
		SkeletalMesh->OnEndCursorOver.AddDynamic(this, &AUNInteractableObjectBase::SendEndDataToController);
	}
}

void AUNInteractableObjectBase::SendBeginDataToController(UPrimitiveComponent* OverlapActor)
{
	if (AUNPlayerController* PC = Cast<AUNPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (this)
		{
			PC->CheckCursorOverObject(this);
		}
	}
}

void AUNInteractableObjectBase::SendEndDataToController(UPrimitiveComponent* OverlapActor)
{
	if (AUNPlayerController* PC = Cast<AUNPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->ClearCursorOverObject(this);
	}
}

void AUNInteractableObjectBase::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}

	if (SkeletalMesh)
	{
		SkeletalMesh->SetRenderCustomDepth(true);
	}
}

void AUNInteractableObjectBase::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}

	if (SkeletalMesh)
	{
		SkeletalMesh->SetRenderCustomDepth(false);
	}
}

void AUNInteractableObjectBase::BeginInteract()
{
}

void AUNInteractableObjectBase::EndInteract()
{
}

void AUNInteractableObjectBase::Interact(AActor* Actor)
{
}
