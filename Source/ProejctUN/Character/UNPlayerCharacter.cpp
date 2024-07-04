// Fill out your copyright notice in the Description page of Project Settings.

#include "UNPlayerCharacter.h"

#include "Player/UNGASPlayerState.h"
#include "Player/UNPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "UI/UNHUD.h"
#include "UI/UNGASWidgetComponent.h"
#include "UI/Widget/UNGASUserWidget.h"
#include "UI/UNInventoryComponent.h"

#include "Components/DecalComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "ASC/UNAbilitySystemComponent.h"
#include "Attribute/UNCharacterAttributeSet.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GameplayTagContainer.h"
#include "Tag/UNGameplayTag.h"

#include "Game/UNWorldSubsystem.h"
#include "Props/UNPickupObject.h"
#include "Item/ItemBase.h"

#include "Props/UNInteractableObjectBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "UNComboActionData.h"

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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInvinsibleRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Invinsible.IA_Invinsible'"));
	if (nullptr != InputActionInvinsibleRef.Object)
	{
		InvinsibleAction = InputActionInvinsibleRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionUltimateRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Ultimate.IA_Ultimate'"));
	if (nullptr != InputActionUltimateRef.Object)
	{
		UltimateAction = InputActionUltimateRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSCancelRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_SCancel.IA_SCancel'"));
	if (nullptr != InputActionSCancelRef.Object)
	{
		SCancelAction = InputActionSCancelRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionConfirmRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Confirm.IA_Confirm'"));
	if (nullptr != InputActionConfirmRef.Object)
	{
		ConfirmAction = InputActionConfirmRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> CancelActionConfirmRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Cancel.IA_Cancel'"));
	if (nullptr != CancelActionConfirmRef.Object)
	{
		CancelAction = CancelActionConfirmRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InventoryActionConfirmRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Inventroy.IA_Inventroy'"));
	if (nullptr != InventoryActionConfirmRef.Object)
	{
		InventoryAction = InventoryActionConfirmRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMenuPanelRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_MenuPanel.IA_MenuPanel'"));
	if (nullptr != InputActionMenuPanelRef.Object)
	{
		MenuPanelAction = InputActionMenuPanelRef.Object;
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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Sword/SK_Blade_BlackKnight.SK_Blade_BlackKnight'"));
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

	Armor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Armor"));
	Armor->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));

	//추후 무기액터의 데이터로 넣을 예정
	WeaponRange = 175.f;
	WeaponAttackRate = 40.f;

	// 스킬 범위 데칼
	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->DecalSize = FVector();
	Decal->SetupAttachment(RootComponent);

	PlayerInventory = CreateDefaultSubobject<UUNInventoryComponent>(TEXT("Inventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.f);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetAutoActivate(false);
	Niagara->SetWorldScale3D(FVector(3.f, 3.f, 3.f));

	HeadNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HeadNiagara"));
	HeadNiagara->SetupAttachment(RootComponent);
	HeadNiagara->SetAutoActivate(false);
	HeadNiagara->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HeadNiagara->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
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
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AUNPlayerCharacter::RightClickAction); //&AUNPlayerCharacter::OnInputStarted
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AUNPlayerCharacter::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AUNPlayerCharacter::OnSetDestinationReleased);

		EnhancedInputComponent->BindAction(MenuPanelAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::MenuPanelFunction);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::InventoryInteraction);
	}
	SetupPlayerGASInputComponent();

	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

// GAS 입력 관련
void AUNPlayerCharacter::SetupPlayerGASInputComponent()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::LeftClickAction); //&AUNPlayerCharacter::GASInputPressed, 0
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(TeleportAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::GASInputPressed, 2);
		EnhancedInputComponent->BindAction(InvinsibleAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::GASInputPressed, 3);
		EnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::GASInputPressed, 4);
		EnhancedInputComponent->BindAction(SCancelAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::SCancelActionFunction);
		EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::SendConfirmToTargetActor);
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Triggered, this, &AUNPlayerCharacter::SendCancelToTargetActor);

		UN_LOG(LogUNNetwork, Log, TEXT("GAS Input Bind Complete"));
	}

	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

void AUNPlayerCharacter::BeginPlay()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	Super::BeginPlay();

	if (PlayerController)
	{
		UN_LOG(LogUNNetwork, Log, TEXT("%s"), TEXT("Have Controller"));
		EnableInput(PlayerController);
	}
	//InitAbilityActorInfo();
	SetCharacterControl();

	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

// 서버만 실행
void AUNPlayerCharacter::PossessedBy(AController* NewController)
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	Super::PossessedBy(NewController);

	PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't find Controller!"));
		return;
	}
	//PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));

	InitAbilityActorInfo();

	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

// 클라이언트만 실행
void AUNPlayerCharacter::OnRep_Owner()
{
	UN_LOG(LogUNNetwork, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));
	Super::OnRep_Owner();

	PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't find Controller!"));
		return;
	}
	//PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));

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

	InitAbilityActorInfo();
	UN_LOG(LogUNNetwork, Log, TEXT("End"));
}

void AUNPlayerCharacter::InitAbilityActorInfo()
{
	PlayerController = Cast<APlayerController>(GetController());

	// Player State
	AUNGASPlayerState* GASPS = GetPlayerState<AUNGASPlayerState>();
	if (GASPS)
	{
		ASC = Cast<UUNAbilitySystemComponent>(GASPS->GetAbilitySystemComponent());
		ASC->InitAbilityActorInfo(GASPS, this);

		//ASC->GenericGameplayEventCallbacks.FindOrAdd(UNTAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &AUNPlayerCharacter::EquipWeapon);
		//ASC->GenericGameplayEventCallbacks.FindOrAdd(UNTAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &AUNPlayerCharacter::UnEquipWeapon);
		ASC->RegisterGameplayTagEvent(UNTAG_CHARACTER_STATE_ISSTUNING, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AUNPlayerCharacter::OnStunTagChange);
		ASC->RegisterGameplayTagEvent(UNTAG_CHARACTER_STATE_ISDEAD, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AUNPlayerCharacter::OnDeadTagChange);

		//PlayerInventory->OnInventoryUpdated.AddUObject(this, &AUNPlayerCharacter::EquipItem);
		if (UUNInventoryComponent* Inven = Cast<UUNInventoryComponent>(PlayerInventory))
		{
			Inven->OnInventoryUpdated.AddUObject(this, &AUNPlayerCharacter::UpdateWeapon); 
			Inven->OnInventoryUpdated.AddUObject(this, &AUNPlayerCharacter::UpdateArmor);
		}
		
	}
	else
	{
		UN_LOG(LogUNNetwork, Log, TEXT("Not Have GAS"));
	}

	InitializeAttributes();
	InitalizeGameplayAbilities();
	//EquipWeapon(nullptr);

	if (AUNPlayerController* PC = Cast<AUNPlayerController>(PlayerController))
	{
		if (HUD = Cast<AUNHUD>(PC->GetHUD()))
		{
			HUD->InitOverlay(PC, GASPS, ASC, GASPS->GetAttributeSet());
			HUD->InitInventory(PC, GASPS, ASC, GASPS->GetAttributeSet());
			HUD->InitProgressBar(PC, GASPS, ASC, GASPS->GetAttributeSet());
		}
	}

	HpBar->InitWidget();
}

void AUNPlayerCharacter::SetCharacterControl()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	if (!IsLocallyControlled())
	{
		return;
	}

	PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't find Controller!"));
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//Subsystem->ClearAllMappings();
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
	if (bisCanceled)
	{
		bisCanceled = false;
		return;
	}

	if (FollowTime <= ShortPressThreshold)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *CachedDestination.ToString());
		if (CursorFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, CursorFX, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, CachedDestination);
	}

	FollowTime = 0.f;
}

// ==================== 이동 관련 ==================== End

void AUNPlayerCharacter::LeftClickAction()
{
	if (bisTargeting)
	{
		SendConfirmToTargetActor();
		return;
	}

	GASInputPressed(0);
}

void AUNPlayerCharacter::RightClickAction()
{
	if (bisTargeting)
	{
		SendCancelToTargetActor();
		bisCanceled = true;
		return;
	}

	if (AUNPlayerController* PC = Cast<AUNPlayerController>(PlayerController))
	{
		PC->BeginInteract();
	}

	//// To Do .. : CollisionChannel
	//FHitResult OutHitResult;
	//if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, OutHitResult))
	//{
	//	if (AUNInteractableObjectBase* Object = Cast<AUNInteractableObjectBase>(OutHitResult.GetActor()))
	//	{
	//		Object->Interact();
	//		bisCanceled = true;
	//		UAIBlueprintHelperLibrary::SimpleMoveToActor(PlayerController, Object);
	//		return;
	//	}
	//}

	OnInputStarted();
}

// ==================== GAS 관련 ==================== Start

// 키 입력시 GA실행
void AUNPlayerCharacter::GASInputPressed(int32 InputId)
{
	if (bisTargeting)
	{
		SendCancelToTargetActor();
		bisCanceled = true;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
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
	// Attribute
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
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));

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

// ==================== GAS 관련 ==================== End

void AUNPlayerCharacter::SendConfirmToTargetActor()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));

	ASC->GetCurrentActiveTargetActor()->ConfirmTargeting();
}

void AUNPlayerCharacter::SendCancelToTargetActor()
{
	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
	
	ASC->GetCurrentActiveTargetActor()->CancelTargeting();
}

void AUNPlayerCharacter::OnStunTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	// 기절 상태 해재
	if (NewCount <= 0)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		StopStunAnimation();

		return;
	}

	// 타겟팅 해제
	if (bisTargeting)
	{
		SendCancelToTargetActor();
	}

	// 기절 상태 시 캔슬되어야 할 어빌리티를 태그를 통해 모두 캔슬
	FGameplayTagContainer CancelTagContainer;
	CancelTagContainer.AddTag(UNTAG_CHARACTER_STATE_ISATTACKING);
	CancelTagContainer.AddTag(UNTAG_CHARACTER_STATE_ISULTIMATING);
	CancelTagContainer.AddTag(UNTAG_CHARACTER_STATE_ISSKILLING);

	FGameplayTagContainer IgnoreTagContainer;
	TArray<FGameplayAbilitySpec*> AbilitySpecs;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(CancelTagContainer, AbilitySpecs, true);
	ASC->CancelAbilities(&CancelTagContainer, &IgnoreTagContainer);
	
	if (!bisDead)
	{
		PlayStunAnimation();
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		return;
	}
	ServerRPCCharacterOnDeath();
}

void AUNPlayerCharacter::OnDeadTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	// 사망 해제 시. 추후 리스폰 요소 추가되면 사용
	if (NewCount <= 0)
	{
		return;
	}
	ServerRPCCharacterOnDeath();
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

void AUNPlayerCharacter::ActivateDecal_Implementation(FDecalStruct DecalStruct)
{
	if (!Decal)
	{
		UE_LOG(LogTemp, Log, TEXT("[null] : Decal"));
		return;
	}

	SetCurrentActiveDecalData_Implementation(DecalStruct);
	Decal->SetMaterial(0, DecalStruct.GetMaterial());
	Decal->SetRelativeLocationAndRotation(DecalStruct.GetLocation(), DecalStruct.GetRotation());
	Decal->DecalSize = DecalStruct.GetScale();

	bisTargeting = true;
}

void AUNPlayerCharacter::EndDecal_Implementation()
{
	if (!Decal)
	{
		UE_LOG(LogTemp, Log, TEXT("[null] : Decal"));
		return;
	}

	Decal->SetMaterial(0, nullptr);
	Decal->DecalSize = FVector();
	ClearCurrentActiveDecalData_Implementation();

	bisTargeting = false;
}

void AUNPlayerCharacter::InventoryInteraction()
{
	HUD->ToggleInventory();
}

void AUNPlayerCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() + 50.f)};
		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(200.f));
		SpawnLocation.Z = GetActorLocation().Z;
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
		
		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		ServerRPCSpawnItem(ItemToDrop->ID, SpawnTransform, QuantityToDrop);
	}

	else
	{
		UE_LOG(LogTemp, Log, TEXT("Item Drop Failed. Item to drop was somehow null"));
	}
}

void AUNPlayerCharacter::UpdateWeapon()
{
	ServerRPCUpdateWeapon();
}

void AUNPlayerCharacter::UpdateArmor()
{
	ServerRPCUpdateArmor();
}

void AUNPlayerCharacter::ServerRPCUpdateWeapon_Implementation()
{
	// 우선 AT를 기본 상태로
	Weapon->SetSkeletalMesh(nullptr);
	ASC->SetUltimateMesh(nullptr);

	const float DefaultAttackRange = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetDefaultAttackRangeAttribute());
	const float DefaultAttackRate = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetDefaultAttackRateAttribute());

	ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute(), DefaultAttackRange);
	ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute(), DefaultAttackRate);

	// 무기 착용 유무 확인
	if (PlayerInventory->CurrentWeaponItemID == NAME_None)
	{
		MulticastRPCUpdateWeapon(nullptr);
		return;
	}

	// 새로운 무기 장착 및 AT업데이트
	if (UUNWorldSubsystem* WorldSubSystem = GetWorld()->GetSubsystem<UUNWorldSubsystem>())
	{
		UItemBase* CurrentEquipItem = WorldSubSystem->GetItemReference(PlayerInventory->CurrentWeaponItemID);
		WeaponMesh = CurrentEquipItem->AssetData.SkeletalMesh;
		Weapon->SetSkeletalMesh(WeaponMesh);
		ASC->SetUltimateMesh(WeaponMesh);

		ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute(), DefaultAttackRange + CurrentEquipItem->ItemStatistics.WeaponRange);
		ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute(), DefaultAttackRate + CurrentEquipItem->ItemStatistics.DamageValue);

		MulticastRPCUpdateWeapon(CurrentEquipItem->AssetData.SkeletalMesh);
	}
}

void AUNPlayerCharacter::ServerRPCUpdateArmor_Implementation()
{
	Armor->SetSkeletalMesh(nullptr);

	const float DefaultArmorRate = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetDefaultArmorRateAttribute());
	ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetArmorRateAttribute(), DefaultArmorRate);

	if (PlayerInventory->CurrentArmorItemID == NAME_None)
	{
		MulticastRPCUpdateArmor(nullptr);
		return;
	}

	if (UUNWorldSubsystem* WorldSubSystem = GetWorld()->GetSubsystem<UUNWorldSubsystem>())
	{
		UItemBase* CurrentEquipItem = WorldSubSystem->GetItemReference(PlayerInventory->CurrentArmorItemID);
		Armor->SetSkeletalMesh(CurrentEquipItem->AssetData.SkeletalMesh);

		ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetArmorRateAttribute(), DefaultArmorRate + CurrentEquipItem->ItemStatistics.ArmorRating);

		MulticastRPCUpdateArmor(CurrentEquipItem->AssetData.SkeletalMesh);
	}
}

void AUNPlayerCharacter::MulticastRPCUpdateWeapon_Implementation(USkeletalMesh* ItemID)
{
	WeaponMesh = ItemID;
	Weapon->SetSkeletalMesh(ItemID);
}

void AUNPlayerCharacter::MulticastRPCUpdateArmor_Implementation(USkeletalMesh* ItemID)
{
	Armor->SetSkeletalMesh(ItemID);
}


void AUNPlayerCharacter::StopMovement()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
}

void AUNPlayerCharacter::ActivateMovement()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AUNPlayerCharacter::ServerRPCDestroyActor_Implementation(AUNPickupObject* Obj)
{
	if (!IsValid(Obj))
	{
		return;
	}

	Obj->MulticastRPCDestroyActor();
}

void AUNPlayerCharacter::ServerRPCSpawnItem_Implementation(FName ID, FTransform SpawnLocaiton, const int32 Quantity)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AUNPickupObject* PickUpObject = GetWorld()->SpawnActor<AUNPickupObject>(AUNPickupObject::StaticClass(), SpawnLocaiton, SpawnParams);

	PickUpObject->InitializeDropItem(ID, Quantity);
}

void AUNPlayerCharacter::UpdateNiagara(UNiagaraSystem* NiagaraSystem)
{
	if (Niagara)
	{
		Niagara->SetAsset(NiagaraSystem);
		Niagara->Activate(true);
	}
}

void AUNPlayerCharacter::UpdateHeadNiagara(UNiagaraSystem* NiagaraSystem)
{
	if (HeadNiagara)
	{
		HeadNiagara->SetAsset(NiagaraSystem);
		HeadNiagara->Activate(true);
	}
}

void AUNPlayerCharacter::SCancelActionFunction()
{
	ServerRPCSCancelActionFunction();

	PlayerController->StopMovement();
}

void AUNPlayerCharacter::StartUltimate(FVector Location)
{
	UltimateLocation = Location;

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(9);
	if (Spec)
	{
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

void AUNPlayerCharacter::ServerRPCSCancelActionFunction_Implementation()
{
	if (ASC->GetCurrentActiveAbility() != nullptr)
	{
		FGameplayAbilitySpecHandle Handle = ASC->GetCurrentActiveAbility()->GetCurrentAbilitySpecHandle();
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);

		ASC->CancelAbilityHandle(Handle);
	}
}

void AUNPlayerCharacter::MenuPanelFunction()
{
	if (bisMenuPanelOpen)
	{
		HUD->CloseEndWidget();
		bisMenuPanelOpen = false;
	}
	else
	{
		HUD->OpenEndWidget(bisDead);
		bisMenuPanelOpen = true;
	}
}

//// 무기 장착. 무기는 추후 액터로 변경
//void AUNPlayerCharacter::EquipWeapon(const FGameplayEventData* EventData)
//{
//	UN_LOG(LogUNNetwork, Log, TEXT("Begin"));
//	if (Weapon)
//	{
//		UN_LOG(LogUNNetwork, Log, TEXT("SetWeaponSkeletalMesh"));
//		Weapon->SetSkeletalMesh(WeaponMesh);
//
//		if (HasAuthority()) //IsLocallyControlled()
//		{
//			//FGameplayAbilitySpec NewSkillSpec(SkillAbilityClass);
//			//NewSkillSpec.InputID = 1;
//
//			//if (!ASC->FindAbilitySpecFromClass(SkillAbilityClass))
//			//{
//			//	ASC->GiveAbility(NewSkillSpec);
//			//}
//
//			const float CurrentAttackRange = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute());
//			const float CurrentAttackRate = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute());
//
//			ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute(), WeaponRange);
//			ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute(), WeaponAttackRate);
//		}
//	}
//	UN_LOG(LogUNNetwork, Log, TEXT("End"));
//}
//
//// 무기 장착 해제. 무기는 추후 액터로 변경
//void AUNPlayerCharacter::UnEquipWeapon(const FGameplayEventData* EventData)
//{
//	if (Weapon)
//	{
//		Weapon->SetSkeletalMesh(nullptr);
//		//FGameplayAbilitySpec* SKillAbilitySpec = ASC->FindAbilitySpecFromClass(SkillAbilityClass);
//
//		//if (SKillAbilitySpec)
//		//{
//		//	ASC->ClearAbility(SKillAbilitySpec->Handle);
//		//}
//
//		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute());
//		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute());
//
//		ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange - WeaponRange);
//		ASC->SetNumericAttributeBase(UUNCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate - WeaponAttackRate);
//	}
//}