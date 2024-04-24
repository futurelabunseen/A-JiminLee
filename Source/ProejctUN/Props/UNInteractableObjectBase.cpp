// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNInteractableObjectBase.h"
#include "Components/StaticMeshComponent.h"
#include "Player/UNPlayerController.h"

AUNInteractableObjectBase::AUNInteractableObjectBase()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AUNInteractableObjectBase::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnBeginCursorOver.AddDynamic(this, &AUNInteractableObjectBase::SendBeginDataToController);
	Mesh->OnEndCursorOver.AddDynamic(this, &AUNInteractableObjectBase::SendEndDataToController);
}

void AUNInteractableObjectBase::SendBeginDataToController(UPrimitiveComponent* OverlapActor)
{
	if (AUNPlayerController* PC = Cast<AUNPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->CheckCursorOverObject(this);
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
}

void AUNInteractableObjectBase::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AUNInteractableObjectBase::BeginInteract()
{
}

void AUNInteractableObjectBase::EndInteract()
{
}

void AUNInteractableObjectBase::Interact()
{
}
