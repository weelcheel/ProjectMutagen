#pragma once

#include "DFPlayerInventory.generated.h"

class ADFInventoryItem;
class ADigitalForgeCharacter;

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

public:

	/**Characters call this so that this Inventory system will call back with weapon data.*/
	void RequestCharacterWeapon(ADigitalForgeCharacter* Character, ADFWeapon* WantedCurrent);
};