// Fill out your copyright notice in the Description page of Project Settings.

#include "UNPlayerCharacter.h"
#include "Player/UNGASPlayerState.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "UNComboActionData.h"
#include "Attribute/UNCharacterAttributeSet.h"
#include "Tag/UNGameplayTag.h"
#include "UI/UNGASWidgetComponent.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"

#include "ProejctUN.h"


AUNPlayerCharacter::AUNPlayerCharacter()
{
	ASC = nullptr;

	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	ShortPressThreshold = 0.3f;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Character.IMC_Character'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		SetDestinationClickAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSkillRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Skill.IA_Skill'"));
	if (nullptr != InputActionSkillRef.Object)
	{
		SkillAction = InputActionSkillRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionTeleportRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Teleport.IA_Teleport'"));
	if (nullptr != InputActionTeleportRef.Object)
	{
		TeleportAction = InputActionTeleportRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionConfirmRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Confirm.IA_Confirm'"));
	if (nullptr != InputActionConfirmRef.Object)
	{
		ConfirmAction = InputActionConfirmRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OutsideAsset/ParagonGreystone/Characters/Heroes/Greystone/Animations/CustomAnimation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UUNComboActionData> ComboActionDataRef(TEXT("/Script/ProejctUN.UNComboActionData'/Game/CharacterAction/UNA_ComboAttackData.UNA_ComboAttackData'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SkillActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OutsideAsset/ParagonGreystone/Characters/Heroes/Greystone/Animations/CustomAnimation/AM_Skill_Q.AM_Skill_Q'"));
	if (SkillActionMontageRef.Object)
	{
		SkillActionMontage = SkillActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_Hellhammer/SK_Blunt_HellHammer.SK_Blunt_HellHammer'"));
	if (WeaponMeshRef.Object)
	{
		WeaponMesh = WeaponMeshRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StunMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/OutsideAsset/ParagonGreystone/Characters/Heroes/Greystone/Animations/CustomAnimation/AM_Stun.AM_Stun'"));
	if (StunMontageRef.Object)
	{
		StunMontage = StunMontageRef.Object;
	}

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	//추후 무기액터의 데이터로 넣을 예정
	WeaponRange = 175.f;
	WeaponAttackRate = 40.f;
}

UAbilitySystemComponent* AUNPlayerCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

// 기본 입력 관련
void AUNPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AUNPlayerCharacter::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AUNPlayerCharacter::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AUNPlayerCharacter::OnSetDestinationReleased);
	}
	SetupPlayerGASInputComponent();

	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

// GAS 입력 관련
void AUNPlayerCharacter::SetupPlayerGASInputComponent()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(TeleportAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::GASInputPressed, 2);
		EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::SendConfirmToTargetActor);

		//ASC->BindToInputComponent(InputComponent);
		//FName Confirmname = "Confirm";
		//FInputActionBinding Confirm = FInputActionBinding(Confirmname, EInputEvent::IE_Pressed);
		//Confirm.ActionDelegate.GetDelegateForManualSet().BindUObject(ASC, &UAbilitySystemComponent::LocalInputConfirm);
		//ASC->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("AbilityID"), static_cast<int32>))
		UN_LOG(LogUNNetwork, Log, TEXT("GAS Input Bind Complete"));
	}

	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

void AUNPlayerCharacter::BeginPlay()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("Have Controller"));
		EnableInput(PlayerController);
	}

	SetCharacterControl();

	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

// 서버만 실행
void AUNPlayerCharacter::PossessedBy(AController* NewController)
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	Super::PossessedBy(NewController);
	
	AUNGASPlayerState* GASPS = GetPlayerState<AUNGASPlayerState>();
	if (GASPS)
	{
		ASC = Cast<UAbilitySystemComponent>(GASPS->GetAbilitySystemComponent());
		ASC->InitAbilityActorInfo(GASPS, this);

		ASC->GenericGameplayEventCallbacks.FindOrAdd(UNTAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &AUNPlayerCharacter::EquipWeapon);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UNTAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &AUNPlayerCharacter::UnEquipWeapon);
		ASC->RegisterGameplayTagEvent(UNTAG_CHARACTER_STATE_ISSTUNING, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AUNPlayerCharacter::OnStunTagChange);
	}
	else
	{
		UN_LOG(LogUNNetwork, Log, TEXT("Not Have GAS"));
	}
	
	PlayerController = CastChecked<APlayerController>(GetController());
	UN_LOG(LogUNNetwork, Log, TEXT("PlayerController : %s"), *PlayerController->GetName());
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));

	InitializeAttributes();
	InitalizeGameplayAbilities();
	EquipWeapon(nullptr);
	HpBar->InitWidget();

	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

// 클라이언트만 실행
void AUNPlayerCharacter::OnRep_Owner()
{
	UN_LOG(LogUNNetwork, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));
	Super::OnRep_Owner();

	PlayerController = CastChecked<APlayerController>(GetController());
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		UN_LOG(LogUNNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("End"));
}

// 클라이언트만 실행
void AUNPlayerCharacter::OnRep_PlayerState()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	Super::OnRep_PlayerState();

	AUNGASPlayerState* GASPS = GetPlayerState<AUNGASPlayerState>();
	if (GASPS)
	{
		ASC = Cast<UAbilitySystemComponent>(GASPS->GetAbilitySystemComponent());
		ASC->InitAbilityActorInfo(GASPS, this);

		ASC->GenericGameplayEventCallbacks.FindOrAdd(UNTAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &AUNPlayerCharacter::EquipWeapon);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UNTAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &AUNPlayerCharacter::UnEquipWeapon);
	}

	InitializeAttributes();
	InitalizeGameplayAbilities();
	SetupPlayerGASInputComponent(); //race condition 방지
	EquipWeapon(nullptr);
	HpBar->InitWidget();

	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

void AUNPlayerCharacter::SetCharacterControl()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	if (!IsLocallyControlled())
	{
		return;
	}

	PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

// ==================== 이동 관련 ==================== Start
void AUNPlayerCharacter::OnInputStarted()
{
	PlayerController->StopMovement();
}

void AUNPlayerCharacter::OnSetDestinationTriggered()
{
	FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	FVector WorldDirection = (CachedDestination - GetActorLocation()).GetSafeNormal();
	AddMovementInput(WorldDirection, 1.0, false);
}

void AUNPlayerCharacter::OnSetDestinationReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, CachedDestination);
	}

	FollowTime = 0.f;
}

// ================================================== End


// ==================== GAS 관련 ==================== Start

// 키 입력시 GA실행
void AUNPlayerCharacter::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		UN_LOG(LogUNNetwork, Log, TEXT("ATTACK!"));
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AUNPlayerCharacter::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

// Attribute 초기화
void AUNPlayerCharacter::InitializeAttributes()
{
	const UUNCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UUNCharacterAttributeSet>();
	if (CurrentAttributeSet)
	{
		// 사망 델리게이트에 함수 등록
		CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &AUNPlayerCharacter::OnOutOfHealth);
	}
}

// GA 초기화. ASC에 GA를 등록하는 단계
void AUNPlayerCharacter::InitalizeGameplayAbilities()
{
	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}

	for (const auto& StartInputAbility : StartInputAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
		StartSpec.InputID = (StartInputAbility.Key);
		ASC->GiveAbility(StartSpec);
	}
}
void AUNPlayerCharacter::SendConfirmToTargetActor()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));

	if (ASC->SpawnedTargetActors.IsEmpty())
	{
		UN_LOG(LogUNNetwork, Log, TEXT("SpawnedTargetActors is Empty"));
		return;
	}

	for (const auto& targetActor : ASC->SpawnedTargetActors)
	{
		//targetActor->ConfirmTargeting();
		//targetActor->ConfirmTargetingAndContinue();
		targetActor->OwningAbility->GetCurrentActorInfo()->AbilitySystemComponent->GenericLocalConfirmCallbacks.Broadcast();
	}
}
// ==================== GAS 관련 ==================== End


// 무기 장착. 무기는 추후 액터로 변경
void AUNPlayerCharacter::EquipWeapon(const FGameplayEventData* EventData)
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	if (Weapon)
	{
		UN_LOG(LogUNNetwork, Log, TEXT("SetWeaponSkeletalMesh"));
		Weapon->SetSkeletalMesh(WeaponMesh);

		if (HasAuthority()) //IsLocallyControlled()
		{
			FGameplayAbilitySpec NewSkillSpec(SkillAbilityClass);
			NewSkillSpec.InputID = 1;

			if (!ASC->FindAbilitySpecFromClass(SkillAbilityClass))
			{
				ASC->GiveAbility(NewSkillSpec);
			}

			const float CurrentAttackRange = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute());
			const float CurrentAttackRate = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute());

			ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute(), WeaponRange);
			ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute(), WeaponAttackRate);
		}
	}
	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

// 무기 장착 해제. 무기는 추후 액터로 변경
void AUNPlayerCharacter::UnEquipWeapon(const FGameplayEventData* EventData)
{
	if (Weapon)
	{
		Weapon->SetSkeletalMesh(nullptr);

		FGameplayAbilitySpec* SKillAbilitySpec = ASC->FindAbilitySpecFromClass(SkillAbilityClass);

		if (SKillAbilitySpec)
		{
			ASC->ClearAbility(SKillAbilitySpec->Handle);
		}

		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute());
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute());

		ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange - WeaponRange);
		ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate - WeaponAttackRate);
	}
}

// 이 아래부터 UNCharacter클래스로 옮길 예정
void AUNPlayerCharacter::OnStunTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		PlayStunAnimation();
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		StopStunAnimation();
	}
}

void AUNPlayerCharacter::PlayStunAnimation_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(StunMontage, 1.f);
}

void AUNPlayerCharacter::StopStunAnimation_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(0.5f, StunMontage);
}
