// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DigitalForgeCharacter.generated.h"

class ADFWeapon;

UCLASS(config=Game)
class ADigitalForgeCharacter : public ACharacter
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:

	ADigitalForgeCharacter(const FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

public:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:

	/**Array of weapons the player currently has but not using. For ease and cosmetic effects.*/
	UPROPERTY(Replicated)
	TArray<ADFWeapon*> HolsteredWeapons;

	UFUNCTION()
	void OnRep_CurrentWeapon(ADFWeapon* lastweapon);

	/** current attacking state */
	uint8 bWantsToAttack : 1;

	UPROPERTY(BlueprintReadOnly, Replicated, Category=Health)
	int32 Health;

	UPROPERTY(BlueprintReadWrite, Category=Health)
	int32 HealthMax;

	UPROPERTY(Replicated)
	int32 VirusUnrest;

	UPROPERTY(Replicated)
	int32 Stamina;

public:

	/**
	 * [local] starts weapon attack
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Attack);
	void StartWeaponAttack();

	/**
	 * [local] stops weapon attack
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Attack);
	void StopWeaponAttack();

	/**Sets the current weapon on the Character. Does not add weapon to the player's Inventory.*/
	UFUNCTION(BlueprintCallable, Category=Inventory)
	void SetCurrentWeapon(ADFWeapon* NewWeapon, ADFWeapon* LastWeapon);

	/**
	 * Adds a holstered weapon to the Character.
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Inventory);
	void AddHolsteredWeapon(ADFWeapon* NewWeapon);

	/**Current weapon the user is currently using.*/
	UPROPERTY(Transient, BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentWeapon)
	ADFWeapon* CurrentWeapon;

	/**
	 * Calculate defensive damage modifers and inflict damage and .
	 */
	UFUNCTION(BlueprintCallable prm1, Category prm2 = Damage);
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	/**
	 * current attacking state
	 */
	uint8 GetWantsToAttack();
	/**
	 * current attacking state
	 */
	void SetWantsToAttack(uint8 newVal);
	int32 GetHealth();
	void SetHealth(int32 newVal);
	int32 GetHealthMax();
	void SetHealthMax(int32 newVal);
	int32 GetStamina();
	void SetStamina(int32 newVal);
	/**
	 * Array of weapons the player currently has but not using. For ease and cosmetic
	 * effects.
	 */
	TArray<ADFWeapon*> GetHolsteredWeapons();
	/**
	 * Array of weapons the player currently has but not using. For ease and cosmetic
	 * effects.
	 */
	void SetHolsteredWeapons(TArray<ADFWeapon*> newVal);
};

