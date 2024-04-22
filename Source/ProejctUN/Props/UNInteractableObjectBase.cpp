// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNInteractableObjectBase.h"
#include "Character/UNPlayerCharacter.h"
#include "UI/UNHUD.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AUNInteractableObjectBase::AUNInteractableObjectBase()
{
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = BoxCollision;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(BoxCollision);
}

void AUNInteractableObjectBase::NotifyActorBeginOverlap(AActor* Other)
{
    if (PlayerCharacter = Cast<AUNPlayerCharacter>(Other))
    {
        bisOverlap = true;
        PlayerCharacter->GetController()->StopMovement();
        if (bisSelected)
        {
            OpenItemPanel();
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Hello Character!");
        }
    }
}

void AUNInteractableObjectBase::NotifyActorEndOverlap(AActor* Other)
{
    if (PlayerCharacter = Cast<AUNPlayerCharacter>(Other))
    {
        bisOverlap = false;

        CloseItemPanel();
        bisSelected = false;

        PlayerCharacter = nullptr;

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Bye Character!");
    }
}

void AUNInteractableObjectBase::Interact()
{
    if (bisOverlap)
    {
        OpenItemPanel();
        return;
    }

    bisSelected = true;
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Interact!");
}

void AUNInteractableObjectBase::OpenItemPanel()
{
    if (AUNHUD* HUD = Cast<AUNHUD>(PlayerCharacter->HUD))
    {
        HUD->OpenInventory();
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Open Panel!");
    // To Do ..
}

void AUNInteractableObjectBase::CloseItemPanel()
{
    if (AUNHUD* HUD = Cast<AUNHUD>(PlayerCharacter->HUD))
    {
        HUD->CloseInventory();
    }
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Close Panel!");
    // To Do ..
}
    