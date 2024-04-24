// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNInteractableObjectBase.h"
#include "Components/StaticMeshComponent.h"

AUNInteractableObjectBase::AUNInteractableObjectBase()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}


void AUNInteractableObjectBase::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnBeginCursorOver.AddDynamic(this, &AUNInteractableObjectBase::TEST);
}

void AUNInteractableObjectBase::TEST(UPrimitiveComponent* PC)
{
	UE_LOG(LogTemp, Log, TEXT("hi"));
}


void AUNInteractableObjectBase::BeginFocus()
{
	UE_LOG(LogTemp, Log, TEXT("BeginFocus"));
}

void AUNInteractableObjectBase::EndFocus()
{
	UE_LOG(LogTemp, Log, TEXT("EndFocus"));
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
