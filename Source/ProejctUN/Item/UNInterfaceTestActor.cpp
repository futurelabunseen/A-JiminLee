// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/UNInterfaceTestActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AUNInterfaceTestActor::AUNInterfaceTestActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

void AUNInterfaceTestActor::BeginFocus()
{
	UE_LOG(LogTemp, Log, TEXT("BeginFocus"));
	//if (Mesh)
	//{
	//	Mesh->SetRenderCustomDepth(true);
	//}
}

void AUNInterfaceTestActor::EndFocus()
{
	UE_LOG(LogTemp, Log, TEXT("EndFocus"));
	//if (Mesh)
	//{
	//	Mesh->SetRenderCustomDepth(false);
	//}
}

void AUNInterfaceTestActor::BeginInteract()
{
	UE_LOG(LogTemp, Log, TEXT("BeginInteract"));
}

void AUNInterfaceTestActor::EndInteract()
{
	UE_LOG(LogTemp, Log, TEXT("EndInteract"));
}

void AUNInterfaceTestActor::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("Interact"));
}