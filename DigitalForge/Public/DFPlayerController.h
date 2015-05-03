#pragma once

#include "DFPlayerController.generated.h"

class ADFPlayerInventory;
class ADFInventoryItem;

UCLASS()
class ADFPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	/**Class that holds the inventory the player has no matter what Character they possess.*/
	UPROPERTY(BlueprintReadOnly, Replicated, Category=Inventory)
	ADFPlayerInventory* PlayerInventory;

	virtual void Possess(APawn* aPawn) override;

public:

	/**Tries to give the player the specified inventory and returns whether or not it succeeded.*/
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool GivePlayerInventory(TSubclassOf<ADFInventoryItem> InventoryClass, bool bTryToActivate);

	/** returns the inventory object */
	ADFPlayerInventory* GetInventory() const;
	/**
	 * Class that holds the inventory the player has no matter what Character they
	 * possess.
	 */
	ADFPlayerInventory* GetPlayerInventory();
	/**
	 * Class that holds the inventory the player has no matter what Character they
	 * possess.
	 */
	void SetPlayerInventory(ADFPlayerInventory* newVal);
};