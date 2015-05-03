#include "DigitalForge.h"
#include "DFEquipItem.h"

ADFEquipItem::ADFEquipItem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}


/**
 * what part of the body the item goes on
 */
TEnumAsByte<EItemEquipBodySlot::Type> ADFEquipItem::GetItemBodySlot(){

	return ItemBodySlot;
}


/**
 * what part of the body the item goes on
 */
void ADFEquipItem::SetItemBodySlot(TEnumAsByte<EItemEquipBodySlot::Type> newVal){

	ItemBodySlot = newVal;
}