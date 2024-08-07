// Fill out your copyright notice in the Description page of Project Settings.


#include "UNCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Physics/UNCollision.h"

#include "Camera/UNSpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UI/UNGASWidgetComponent.h"
#include "UI/Widget/UNGASUserWidget.h"

#include "Game/UNGameMode.h"
#include "EngineUtils.h"
#include "ProejctUN.h"

AUNCharacter::AUNCharacter()
{
	//기본 세팅
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_UNCAPSULE);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 움직임 세팅
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 스프링 암
	CameraBoom = CreateDefaultSubobject<UUNSpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, -45.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// 카메라
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, UUNSpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	// 메쉬
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	// 프로퍼티 기본값
	DeadEventDelayTime = 10.f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/OutsideAsset/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone_NotWeapon.Greystone_NotWeapon'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/OutsideAsset/ParagonGreystone/Characters/Heroes/Greystone/Animations/CustomAnimation/ABP_UNCharacter.ABP_UNCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OutsideAsset/ParagonGreystone/Characters/Heroes/Greystone/Animations/CustomAnimation/AM_Death.AM_Death'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// HpBar 선언 후 위치 설정
	HpBar = CreateDefaultSubobject<UUNGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 250.f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AUNCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AUNCharacter::ServerRPCCharacterOnDeath_Implementation()
{
	OnOutOfHealth();
}

// 델리게이트에 등록되는 함수. 사망 시 Attribute에서 Broadcast됨.
void AUNCharacter::OnOutOfHealth()
{
	MulticastRPCPlayAnimation(this);

	OnDeath.Broadcast(this);
}

// 멀티캐스트로 모든 플레이어에게 전달
void AUNCharacter::MulticastRPCPlayAnimation_Implementation(AUNCharacter* Character)
{
	SetDead();
}

// TakeDamage함수. 지금은 GE를 통해서 데미지 전달. 추후 삭제 예정

//float AUNCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
//	
//	SetDead();
//
//	return Damage;
//}

// 사망 시 실행되는 함수
void AUNCharacter::SetDead()
{
	bisDead = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimaition();
}

// 사망 애니메이션 재생
void AUNCharacter::PlayDeadAnimaition()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(DeadMontage, 1.f);
}




