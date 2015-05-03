#include "DigitalForge.h"
#include "DFPlayerController.h"
#include "DigitalForgeCharacter.h"
#include "DFPlayerInventory.h"
#include "DFInventoryItem.h"
#include "DFWeapon.h"
#include "UnrealNetwork.h"
#include "DFSkill.h"

ADFPlayerController::ADFPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

bool ADFPlayerController::GivePlayerInventory(TSubclassOf<ADFInventoryItem> InventoryClass, bool bTryToActivate)
{
	if (!GetCharacter())
	{
		return false;
	}

	//this entire function could probably be rewritten to look cleaner

	bool bSucceeded = false;
	ADigitalForgeCharacter* pc = Cast<ADigitalForgeCharacter>(GetCharacter());

	//if were adding a skill
	if (InventoryClass->IsChildOf(ADFSkill::StaticClass()))
	{
		FActorSpawnParameters par;
		par.bNoCollisionFail = true;
		ADFSkill* sk = GetWorld()->SpawnActor<ADFSkill>(InventoryClass, GetCharacter()->GetActorLocation(), GetCharacter()->GetActorRotation(), par);

		if (sk)
		{
			PlayerInventory->KnownSkills.AddUnique(sk);
			sk->OnEnterInventory(pc);
		}

		return true;
	}

	FActorSpawnParameters par;
	par.bNoCollisionFail = true;
	ADFWeapon* wp = GetWorld()->SpawnActor<ADFWeapon>(InventoryClass, GetCharacter()->GetActorLocation(), GetCharacter()->GetActorRotation(), par);

	if (wp) //adding weapon
	{
		PlayerInventory->PlayerWeapons.AddUnique(wp);
		wp->OnEnterInventory(pc);

		if (bTryToActivate)
		{
			if (pc)
			{
				PlayerInventory->RequestCharacterWeapon(pc, wp);
			}
		}

		bSucceeded = true;
	}
	else //other inventory item
	{
		PlayerInventory->InventoryItems.AddUnique(InventoryClass);
		bSucceeded = true;
	}

	return bSucceeded;
}

void ADFPlayerController::Possess(APawn* aPawn)
{
	FActorSpawnParameters par;
	par.bNoCollisionFail = true;
	par.Instigator = GetCharacter();
	par.Owner = this;
	PlayerInventory = GetWorld()->SpawnActor<ADFPlayerInventory>(ADFPlayerInventory::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, par);

	Super::Possess(aPawn);
}

void ADFPlayerController::GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADFPlayerController, PlayerInventory);
}

ADFPlayerInventory* ADFPlayerController::GetInventory() const
{
	return PlayerInventory;
}