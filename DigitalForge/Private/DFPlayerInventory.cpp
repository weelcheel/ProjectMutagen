#include "DigitalForge.h"
#include "DFInventoryItem.h"
#include "DigitalForgeCharacter.h"
#include "DFSkill.h"
#include "DFPlayerInventory.h"
#include "UnrealNetwork.h"

ADFPlayerInventory::ADFPlayerInventory(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

/**
 * Characters call this so that this Inventory system will call back with weapon
 * data.
 */
void ADFPlayerInventory::RequestCharacterWeapon(ADigitalForgeCharacter* Character, ADFWeapon* WantedCurrent)
{
	if (WantedCurrent)
	{
		bool bHasInInventory = false; //make sure the weapon passed is in the inventory
		for (int32 i = 0; i < PlayerWeapons.Num(); i++)
		{
			if (PlayerWeapons[i] == WantedCurrent)
			{
				Character->SetCurrentWeapon(WantedCurrent, Character->CurrentWeapon);
				bHasInInventory = true;
			}
		}

		if (!bHasInInventory)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory: Tried to request a weapon that doesn't exist in player's inventory."));
		}
	}
	else //bad or no wanted weapon, so switch to the first one in the array
	{
		if (PlayerWeapons[0])
		{
			Character->SetCurrentWeapon(PlayerWeapons[0], Character->CurrentWeapon);
		}
	}
}

void ADFPlayerInventory::GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADFPlayerInventory, PlayerWeapons);
	DOREPLIFETIME(ADFPlayerInventory, InventoryItems);
	DOREPLIFETIME(ADFPlayerInventory, KnownSkills);
}

/**
 * test skill system. debug use only.
 */
ADFSkill* ADFPlayerInventory::GetDebugSkill()
{
	return KnownSkills[0];
}


/**
 * Weapons the player currently has in inventory.
 */
TArray<ADFWeapon*> ADFPlayerInventory::GetPlayerWeapons(){

	return PlayerWeapons;
}


/**
 * Weapons the player currently has in inventory.
 */
void ADFPlayerInventory::SetPlayerWeapons(TArray<ADFWeapon*> newVal){

	PlayerWeapons = newVal;
}


/**
 * Array of skills that the player currently knows.
 */
TArray<ADFSkill*> ADFPlayerInventory::GetKnownSkills(){

	return KnownSkills;
}


/**
 * Array of skills that the player currently knows.
 */
void ADFPlayerInventory::SetKnownSkills(TArray<ADFSkill*> newVal){

	KnownSkills = newVal;
}


/**
 * Other items in the inventory array.
 */
TArray<TSubclassOf<ADFInventoryItem> > ADFPlayerInventory::GetInventoryItems(){

	return InventoryItems;
}


/**
 * Other items in the inventory array.
 */
void ADFPlayerInventory::SetInventoryItems(TArray<TSubclassOf<ADFInventoryItem> > newVal){

	InventoryItems = newVal;
}