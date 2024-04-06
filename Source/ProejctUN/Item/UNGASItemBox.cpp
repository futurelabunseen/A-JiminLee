// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/UNGASItemBox.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/UNCollision.h"
#include "AbilitySystemBlueprintLibrary.h"

AUNGASItemBox::AUNGASItemBox()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	
	Trigger->SetCollisionProfileName(CPROFILE_UNTRIGGER);
	Trigger->SetBoxExtent(FVector(40.f, 42.f, 30.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_CornerFrame.SM_CornerFrame'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}

	Mesh->SetRelativeLocation(FVector(-30.f, -3.5f, -30.f));
	Mesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	bReplicates = true;
	bAlwaysRelevant = true;
}

UAbilitySystemComponent* AUNGASItemBox::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AUNGASItemBox::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	InvokeGameplayCue(Other);
	ApplyEffectToTarget(Other);

	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	SetLifeSpan(2.f);
}

void AUNGASItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ASC->InitAbilityActorInfo(this, this);
}

void AUNGASItemBox::ApplyEffectToTarget(AActor* Target)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void AUNGASItemBox::InvokeGameplayCue(AActor* Target)
{
	FGameplayCueParameters Param;
	Param.SourceObject = this;
	Param.Instigator = Target;
	Param.Location = GetActorLocation();
	ASC->ExecuteGameplayCue(GameplayCueTag, Param);
}


