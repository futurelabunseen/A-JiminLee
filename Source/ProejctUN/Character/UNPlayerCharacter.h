// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UNCharacter.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemInterface.h"
#include "Struct/DecalStruct.h"
#include "UNPlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UUNInventoryComponent;
class AUNHUD;
class UItemBase;

/**
 * 
 */
UCLASS()
class PROEJCTUN_API AUNPlayerCharacter : public AUNCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AUNPlayerCharacter();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE virtual class UAnimMontage* GetComboActionMontage() const { return ComboActionMontage; }
	FORCEINLINE virtual class UAnimMontage* GetSkillActionMontage() const { return SkillActionMontage; }
	FORCEINLINE class UUNComboActionData* GetComboActionData() const { return ComboActionData; }
	FORCEINLINE UUNInventoryComponent* GetInventoryComponent() const { return PlayerInventory; }

	UPROPERTY()
	TObjectPtr<AUNHUD> HUD;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> ConfirmCancelMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TeleportAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InvinsibleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UltimateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ConfirmAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CancelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* Niagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* HeadNiagara;


	UPROPERTY(EditAnywhere)
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Owner() override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();

	void InitAbilityActorInfo();

	void SetCharacterControl();
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	void LeftClickAction();
	void RightClickAction();

	uint8 bisTargeting;
	uint8 bisCanceled;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUNComboActionData> ComboActionData;

// UnUPROPERTY variable
private:
	FVector CachedDestination;

	float FollowTime;

// Weapon
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Armor;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<class USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<class USkeletalMesh> ArmorMesh;

protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	float WeaponRange;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	float WeaponAttackRate;

	void EquipWeapon(const FGameplayEventData* EventData);
	void UnEquipWeapon(const FGameplayEventData* EventData);

	void OnStunTagChange(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION(NetMulticast, Unreliable)
	void PlayStunAnimation();

	UFUNCTION(NetMulticast, Unreliable)
	void StopStunAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> StunMontage;

// GAS
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UUNAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap < int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayAbility> SkillAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SkillActionMontage;

	void SetupPlayerGASInputComponent();

	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	void InitializeAttributes();
	void InitalizeGameplayAbilities();

	void SendConfirmToTargetActor();
	void SendCancelToTargetActor();

// Decal
private:
	UPROPERTY()
	FDecalStruct CurrentActiveDecalData;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateDecal(FDecalStruct DecalStruct);

	UFUNCTION(BlueprintCallable)
	void EndDecal();

	UFUNCTION(BlueprintCallable)
	FDecalStruct GetCurrentActiveDecalData() { return CurrentActiveDecalData; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentActiveDecalData(FDecalStruct NewDecalData) { CurrentActiveDecalData = NewDecalData; }

	UFUNCTION(BlueprintCallable)
	void ClearCurrentActiveDecalData() { CurrentActiveDecalData = FDecalStruct(); }

// UI
public:
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UUNInventoryComponent* PlayerInventory;

	void InventoryInteraction();
	
	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);
	
	UFUNCTION()
	void UpdateWeapon();

	UFUNCTION()
	void UpdateArmor();

	UFUNCTION(Server, Unreliable)
	void ServerRPCUpdateWeapon();

	UFUNCTION(Server, Unreliable)
	void ServerRPCUpdateArmor();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCUpdateWeapon(USkeletalMesh* ItemID);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCUpdateArmor(USkeletalMesh* ItemID);

	UFUNCTION()
	void StopMovement();

	UFUNCTION()
	void ActivateMovement();

	UFUNCTION(Server, Unreliable)
	void ServerRPCSpawnItem(FName ID, FTransform SpawnLocation, const int32 Quantity);

	UFUNCTION(Server, Unreliable)
	void ServerRPCDestoryActor(AUNPickupObject* Obj);

// Niagara
public:
	UFUNCTION()
	void UpdateNiagara(UNiagaraSystem* NiagaraSystem);

	UFUNCTION()
	void UpdateHeadNiagara(UNiagaraSystem* NiagaraSystem);
};
