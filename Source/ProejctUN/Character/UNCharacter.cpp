// Fill out your copyright notice in the Description page of Project Settings.


#include "UNCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Physics/UNCollision.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UI/UNGASWidgetComponent.h"
#include "UI/UNGASUserWidget.h"
#include "EngineUtils.h"
#include "ProejctUN.h"

AUNCharacter::AUNCharacter()
{
	bReplicates = true;

	//�⺻ ����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_UNCAPSULE);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ĳ���� ������ ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// ������ ��
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, -45.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// ī�޶�
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	// �޽�
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	// ������Ƽ �⺻��
	DeadEventDelayTime = 5.f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone_NotWeapon.Greystone_NotWeapon'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/CustomAnimation/ABP_UNCharacter.ABP_UNCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/CustomAnimation/AM_Death.AM_Death'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// HpBar ���� �� ��ġ ����
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

// ��������Ʈ�� ��ϵǴ� �Լ�. ��� �� Attribute���� Broadcast��.
void AUNCharacter::OnOutOfHealth()
{
	MulticastRPCPlayAnimation(this);

	//�ϴ� ��Ƽĳ��Ʈ�� ����
	
	//for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	//{
	//	if (PlayerController && GetController() != PlayerController)
	//	{
	//		if (!PlayerController->IsLocalController())
	//		{
	//			AUNCharacter* OtherPlayer = Cast<AUNCharacter>(PlayerController->GetPawn());
	//			if (OtherPlayer)
	//			{
	//				OtherPlayer->ClientRPCPlayAnimation(this);
	//			}
	//		}
	//	}
	//}
}

// ��Ƽĳ��Ʈ�� ��� �÷��̾�� ����
void AUNCharacter::MulticastRPCPlayAnimation_Implementation(AUNCharacter* Character)
{
	SetDead();
}

// TakeDamage�Լ�. ������ GE�� ���ؼ� ������ ����. ���� ���� ����

//float AUNCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
//	
//	SetDead();
//
//	return Damage;
//}

// ��� �� ����Ǵ� �Լ�
void AUNCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimaition();
	SetActorEnableCollision(false);

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&]
		{
			Destroy();
		}), DeadEventDelayTime, false);
}

// ��� �ִϸ��̼� ���
void AUNCharacter::PlayDeadAnimaition()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(DeadMontage, 1.f);
}




