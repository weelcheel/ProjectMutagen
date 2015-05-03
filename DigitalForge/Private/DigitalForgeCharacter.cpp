// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "DigitalForge.h"
#include "DigitalForgeCharacter.h"
#include "DFWeapon.h"
#include "Stat.h"
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

void ADigitalForgeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ADigitalForgeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ADigitalForgeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADigitalForgeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

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
	DOREPLIFETIME(ADigitalForgeCharacter, BaseStats);
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