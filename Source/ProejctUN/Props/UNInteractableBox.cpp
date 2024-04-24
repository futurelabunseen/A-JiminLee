// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNInteractableBox.h"
#include "Character/UNPlayerCharacter.h"
#include "UI/UNHUD.h"
#include "Components/BoxComponent.h"


AUNInteractableBox::AUNInteractableBox()
{
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = BoxCollision;
    Mesh->SetupAttachment(BoxCollision);
}

void AUNInteractableBox::NotifyActorBeginOverlap(AActor* Other)
{
    if (PlayerCharacter = Cast<AUNPlayerCharacter>(Other))
    {
        bisOverlap = true;
        PlayerCharacter->GetController()->StopMovement();
        if (bisSelected)
        {
            OpenItemPanel();
        }
    }
}

void AUNInteractableBox::NotifyActorEndOverlap(AActor* Other)
{
    if (PlayerCharacter = Cast<AUNPlayerCharacter>(Other))
    {
        bisOverlap = false;

        CloseItemPanel();
        bisSelected = false;
        PlayerCharacter = nullptr;
    }
}

void AUNInteractableBox::Interact()
{
    Super::Interact();

    if (bisOverlap)
    {
        OpenItemPanel();
        return;
    }

    bisSelected = true;
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "1");
}

void AUNInteractableBox::OpenItemPanel()
{
    if (AUNHUD* HUD = Cast<AUNHUD>(PlayerCharacter->HUD))
    {
        HUD->OpenBoxInventory();
        HUD->OpenInventory();
    }
    // To Do ..
}

void AUNInteractableBox::CloseItemPanel()
{
    if (AUNHUD* HUD = Cast<AUNHUD>(PlayerCharacter->HUD))
    {
        HUD->CloseBoxInventory();
        HUD->CloseInventory();
    }
}