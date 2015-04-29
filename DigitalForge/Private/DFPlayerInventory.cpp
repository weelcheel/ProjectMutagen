#include "DigitalForge.h"
#include "DFInventoryItem.h"
#include "DigitalForgeCharacter.h"
#include "DFPlayerInventory.h"
#include "UnrealNetwork.h"

ADFPlayerInventory::ADFPlayerInventory(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ADFPlayerInventory::RequestCharacterWeapon(ADigitalForgeCharacter* Character, ADFWeapon* WantedCurrent)
{
	if (WantedCurrent)
	{
		bool bHasInInventory = false; //make sure the weapon passed is in the inventory
		for (int32 i = 0; i < PlayerWeapons.Num(); i++)
		{
			if (PlayerWeapons[i] == WantedCurrent)
				Character->SetCurrentWeapon(WantedCurrent, Character->CurrentWeapon);
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
	DOREPLIFETIME(ADFPlayerInventory, InventoryItems)
}