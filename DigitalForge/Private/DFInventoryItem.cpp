#include "DigitalForge.h"
#include "DFInventoryItem.h"

ADFInventoryItem::ADFInventoryItem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

FString ADFInventoryItem::GetItemName()
{
	return ItemName;
}