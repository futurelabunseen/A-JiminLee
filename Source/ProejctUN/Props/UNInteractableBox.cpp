// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNInteractableBox.h"
#include "Character/UNPlayerCharacter.h"
#include "UI/UNHUD.h"


AUNInteractableBox::AUNInteractableBox()
{

}

void AUNInteractableBox::NotifyActorBeginOverlap(AActor* Other)
{
    if (PlayerCharacter = Cast<AUNPlayerCharacter>(Other))
    {
        if (PlayerCharacter->IsLocallyControlled())
        {
            bisOverlap = true;
            PlayerCharacter->GetController()->StopMovement();
            if (bisSelected)
            {
                OpenItemPanel();
            }
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

void AUNInteractableBox::Interact(AActor* Actor)
{
    Super::Interact(Actor);

    if (bisOverlap)
    {
        OpenItemPanel();
        return;
    }

    bisSelected = true;
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