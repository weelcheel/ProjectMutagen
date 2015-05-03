// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "DigitalForge.h"
#include "DigitalForgeCharacter.h"
#include "DFWeapon.h"
#include "UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// ADigitalForgeCharacter

ADigitalForgeCharacter::ADigitalForgeCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Health = HealthMax;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ADigitalForgeCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = HealthMax;
}

/**
 * Handler for when a touch input begins.
 */
void ADigitalForgeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

/**
 * Handler for when a touch input stops.
 */
void ADigitalForgeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

/**
 * Called via input to turn at a given rate.
 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
 */
void ADigitalForgeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

/**
 * Called via input to turn look up/down at a given rate.
 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
 */
void ADigitalForgeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/**
 * Called for forwards/backward input
 */
void ADigitalForgeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

/**
 * Called for side to side input
 */
void ADigitalForgeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ADigitalForgeCharacter::SetCurrentWeapon(ADFWeapon* NewWeapon, ADFWeapon* LastWeapon)
{
	ADFWeapon* LocalLastWeapon = NULL;

	if (LastWeapon != NULL)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
	}

	CurrentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);
		NewWeapon->OnEquip();
	}
}

void ADigitalForgeCharacter::AddHolsteredWeapon(ADFWeapon* NewWeapon)
{

}

void ADigitalForgeCharacter::OnRep_CurrentWeapon(ADFWeapon* lastweapon)
{
	SetCurrentWeapon(CurrentWeapon, lastweapon);
}

void ADigitalForgeCharacter::GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADigitalForgeCharacter, CurrentWeapon);
	DOREPLIFETIME(ADigitalForgeCharacter, Health);
}

void ADigitalForgeCharacter::StartWeaponAttack()
{
	if (!bWantsToAttack)
	{
		bWantsToAttack = true;
		UE_LOG(LogTemp, Warning, TEXT("Starting weapona attack!"));
		if (CurrentWeapon)
		{
			CurrentWeapon->StartAttack();
		}
	}
}

void ADigitalForgeCharacter::StopWeaponAttack()
{
	if (bWantsToAttack)
	{
		bWantsToAttack = false;
		UE_LOG(LogTemp, Warning, TEXT("Stopping weapona attack!"));
		if (CurrentWeapon)
		{
			CurrentWeapon->StopAttack();
		}
	}
}

float ADigitalForgeCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//@todo: implement all defensive stat modifers

	Health -= Damage;

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}


/**
 * current attacking state
 */
uint8 ADigitalForgeCharacter::GetWantsToAttack(){

	return bWantsToAttack;
}


/**
 * current attacking state
 */
void ADigitalForgeCharacter::SetWantsToAttack(uint8 newVal){

	bWantsToAttack = newVal;
}


int32 ADigitalForgeCharacter::GetHealth(){

	return Health;
}


void ADigitalForgeCharacter::SetHealth(int32 newVal){

	Health = newVal;
}


int32 ADigitalForgeCharacter::GetHealthMax(){

	return HealthMax;
}


void ADigitalForgeCharacter::SetHealthMax(int32 newVal){

	HealthMax = newVal;
}


int32 ADigitalForgeCharacter::GetStamina(){

	return Stamina;
}


void ADigitalForgeCharacter::SetStamina(int32 newVal){

	Stamina = newVal;
}


/**
 * Array of weapons the player currently has but not using. For ease and cosmetic
 * effects.
 */
TArray<ADFWeapon*> ADigitalForgeCharacter::GetHolsteredWeapons(){

	return HolsteredWeapons;
}


/**
 * Array of weapons the player currently has but not using. For ease and cosmetic
 * effects.
 */
void ADigitalForgeCharacter::SetHolsteredWeapons(TArray<ADFWeapon*> newVal){

	HolsteredWeapons = newVal;
}