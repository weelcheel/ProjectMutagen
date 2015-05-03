#pragma once

#include "DFPlayerInventory.generated.h"

class ADFInventoryItem;
class ADigitalForgeCharacter;
class ADFSkill;

UCLASS()
class ADFPlayerInventory : public AActor
{
	GENERATED_UCLASS_BODY()

	/**Other items in the inventory array.*/
	UPROPERTY(Replicated)
	TArray<TSubclassOf<ADFInventoryItem> > InventoryItems;

	/**Weapons the player currently has in inventory.*/
	UPROPERTY(Replicated)
	TArray<ADFWeapon*> PlayerWeapons;

	/**Array of skills that the player currently knows.*/
	UPROPERTY(Replicated)
	TArray<ADFSkill*> KnownSkills;

public:

	/**Characters call this so that this Inventory system will call back with weapon data.*/
	void RequestCharacterWeapon(ADigitalForgeCharacter* Character, ADFWeapon* WantedCurrent);

	/** test skill system. debug use only. */
	ADFSkill* GetDebugSkill();
	/**
	 * Weapons the player currently has in inventory.
	 */
	TArray<ADFWeapon*> GetPlayerWeapons();
	/**
	 * Weapons the player currently has in inventory.
	 */
	void SetPlayerWeapons(TArray<ADFWeapon*> newVal);
	/**
	 * Array of skills that the player currently knows.
	 */
	TArray<ADFSkill*> GetKnownSkills();
	/**
	 * Array of skills that the player currently knows.
	 */
	void SetKnownSkills(TArray<ADFSkill*> newVal);
	/**
	 * Other items in the inventory array.
	 */
	TArray<TSubclassOf<ADFInventoryItem> > GetInventoryItems();
	/**
	 * Other items in the inventory array.
	 */
	void SetInventoryItems(TArray<TSubclassOf<ADFInventoryItem> > newVal);
};