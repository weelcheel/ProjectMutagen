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
};