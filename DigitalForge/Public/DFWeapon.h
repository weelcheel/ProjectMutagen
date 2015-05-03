#pragma once

#include "DFInventoryItem.h"
#include "DFWeapon.generated.h"

namespace EWeaponState
{
	enum Type
	{
		Idle,
		Attacking,
		Recharging,
		Equipping,
		Unequiping,
	};
}

class ADigitalForgeCharacter;

UCLASS(ABSTRACT, Blueprintable)
class ADFWeapon : public ADFInventoryItem
{
	GENERATED_UCLASS_BODY()

	/** perform initial setup */
	virtual void PostInitializeComponents() override;

	/** weapon is being equipped by owner pawn */
	virtual void OnEquip();

	/** weapon is now equipped by owner pawn */
	virtual void OnEquipFinished();

	/** weapon is holstered by owner pawn */
	virtual void OnUnEquip();

	virtual void OnLeaveInventory() override;

	/** check if it's currently equipped */
	bool IsEquipped() const;

	/** check if mesh is already attached */
	bool IsAttachedToPawn() const;

	/** [local + server] start weapon fight */
	virtual void StartAttack();

	/** [local + server] stop weapon fight */
	virtual void StopAttack();

	/** check if weapon can fight */
	bool CanAttack() const;

	/** gets last time when this weapon was switched to */
	float GetEquipStartedTime() const;

	/** gets the duration of equipping weapon*/
	float GetEquipDuration() const;

	/**Weapon mesh.*/
	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* WeaponMesh;

protected:

	/** fighting audio */
	UPROPERTY(Transient)
	UAudioComponent* AttackAudio;

	/**equip anim for Character mesh*/
	UPROPERTY(EditDefaultsOnly, Category=Animation)
	UAnimMontage* EquipAnimation;

	/**attack anim for Character mesh*/
	UPROPERTY(EditDefaultsOnly, Category=Animation)
	UAnimMontage* AttackAnimation;

	/** is attack animation playing? */
	uint32 bPlayingAttackAnim : 1;

	/** is weapon currently equipped? */
	uint32 bIsEquipped : 1;

	/** is weapon attack active? */
	uint32 bWantsToAttack : 1;

	/** is equip animation playing? */
	uint32 bPendingEquip : 1;

	/** weapon is reattacking */
	uint32 bReattacking;

	/** current weapon state */
	EWeaponState::Type CurrentState;

	/** time of last successful weapon attack */
	float LastAttackTime;

	/** last time when this weapon was switched to */
	float EquipStartedTime;

	/** how much time weapon needs to be equipped */
	float EquipDuration;

	/**
	 * [server] attack & update
	 */
	UFUNCTION(reliable, server, WithValidation)
	void ServerStartAttack();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopAttack();

	UFUNCTION(BlueprintImplementableEvent, Category=Weapon)
	void WeaponAttack();

	/** [server] attack & update */
	UFUNCTION(reliable, server, WithValidation)
	void ServerHandleAttack();
	void HandleAttack();

	/** update weapon state */
	void SetWeaponState(EWeaponState::Type NewState);

	/** determine current weapon state */
	void DetermineWeaponState();

	/** attaches weapon mesh to pawn's mesh */
	void AttachMeshToPawn();

	/** detaches weapon mesh from pawn */
	void DetachMeshFromPawn();

	/**play weapon sounds*/
	UAudioComponent* PlayWeaponSound(USoundCue* Sound);
	float PlayWeaponAnimation(UAnimMontage* Animation);

	/**
	 * play weapon animations
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Animation);
	void StopWeaponAnimation(UAnimMontage* Animation);

	/**
	 * [local + server] handle weapon attack
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Functionality);
	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	/** [local + server] attack started */
	virtual void OnAttackStarted();

	/** [local + server] attack finished */
	virtual void OnAttackFinished();

	/**
	 * stop playing weapon animations
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Animation);

	/**
	 * find hit
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Trace);

	UPROPERTY(BlueprintReadWrite, Category=Range)
	float WeaponRange;

public:

	/** get current weapon state */
	EWeaponState::Type GetCurrentState() const;
	USkeletalMeshComponent* GetWeaponMesh() const;
	bool GetWeaponWantsToAttack();

	/**
	 * get weapon mesh (needs pawn owner to determine variant)
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Mesh);

	/**
	 * get whether or not the weapon is attacking
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Attack);
	float GetWeaponRange();
	void SetWeaponRange(float newVal);
	/**
	 * time of last successful weapon attack
	 */
	float GetLastAttackTime();
	/**
	 * time of last successful weapon attack
	 */
	void SetLastAttackTime(float newVal);
	/**
	 * equip anim for Character mesh
	 */
	UAnimMontage* GetEquipAnimation();
	/**
	 * equip anim for Character mesh
	 */
	void SetEquipAnimation(UAnimMontage* newVal);
	/**
	 * is weapon attack active?
	 */
	uint32 GetWantsToAttack();
	/**
	 * is weapon attack active?
	 */
	void SetWantsToAttack(uint32 newVal);
	/**
	 * weapon is reattacking
	 */
	uint32 GetReattacking();
	/**
	 * weapon is reattacking
	 */
	void SetReattacking(uint32 newVal);
	/**
	 * is attack animation playing?
	 */
	uint32 GetPlayingAttackAnim();
	/**
	 * is attack animation playing?
	 */
	void SetPlayingAttackAnim(uint32 newVal);
	/**
	 * is equip animation playing?
	 */
	uint32 GetPendingEquip();
	/**
	 * is equip animation playing?
	 */
	void SetPendingEquip(uint32 newVal);
	/**
	 * is weapon currently equipped?
	 */
	uint32 GetIsEquipped();
	/**
	 * is weapon currently equipped?
	 */
	void SetIsEquipped(uint32 newVal);
	/**
	 * fighting audio
	 */
	UAudioComponent* GetAttackAudio();
	/**
	 * fighting audio
	 */
	void SetAttackAudio(UAudioComponent* newVal);
	/**
	 * attack anim for Character mesh
	 */
	UAnimMontage* GetAttackAnimation();
	/**
	 * attack anim for Character mesh
	 */
	void SetAttackAnimation(UAnimMontage* newVal);
};