// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNUltimateSword.h"

// Sets default values
AUNUltimateSword::AUNUltimateSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AUNUltimateSword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUNUltimateSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}