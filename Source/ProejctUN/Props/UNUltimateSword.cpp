// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/UNUltimateSword.h"
#include "Physics/UNCollision.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AUNUltimateSword::AUNUltimateSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(TEXT("HitActor"));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

UAbilitySystemComponent* AUNUltimateSword::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AUNUltimateSword::NotifyActorBeginOverlap(AActor* Other)
{
	if (!Cast<AUNUltimateSword>(Other))
	{
		InvokeGameplayCue(Other);
		ApplyEffectToTarget(Other);
		//Mesh->SetHiddenInGame(true);
		SetActorEnableCollision(false);
		SetLifeSpan(2.f);
	}
}

void AUNUltimateSword::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponMesh)
	{
		Mesh->SetSkeletalMesh(WeaponMesh);
	}
}

void AUNUltimateSword::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(AUNUltimateSword, WeaponMesh, COND_None, REPNOTIFY_Always);
}

void AUNUltimateSword::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ASC->InitAbilityActorInfo(this, this);
}

void AUNUltimateSword::ApplyEffectToTarget(AActor* Target)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			// 타겟의 ASC에서 GE 실행
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void AUNUltimateSword::InvokeGameplayCue(AActor* Target)
{
	if(HasAuthority())
	{
		FGameplayCueParameters Param;
		Param.SourceObject = this;
		Param.Instigator = Target;

		FVector Start = GetActorLocation();
		FVector End = Start - FVector(0.0f, 0.0f, 2000.0f);

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 1.0f);

		if (bHit)
		{
			UE_LOG(LogTemp, Log, TEXT("FoundLocation"));
			Param.Location = HitResult.Location + FVector(0.f, 0.f ,5.f);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("NotFoundLocation"));
			Param.Location = GetActorLocation();
		}
		ASC->ExecuteGameplayCue(GameplayCueTag, Param);
	}
}

void AUNUltimateSword::SetMeshToWeaponMesh(USkeletalMesh* NewMesh)
{
	WeaponMesh = NewMesh;
	//Mesh->SetSkeletalMesh(NewMesh);
}
