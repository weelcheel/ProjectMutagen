#pragma once

#include "DFInventoryItem.h"
#include "DFEquipItem.generated.h"

UENUM(BlueprintType)
namespace EItemEquipBodySlot
{
	enum Type
	{
		Head,
		Torso,
		Shoulder,
		Arm,
		Leg,
		Foot,
	};
}

UCLASS(Blueprintable)
class ADFEquipItem : public ADFInventoryItem
{
	GENERATED_UCLASS_BODY()

	/** what part of the body the item goes on */
	UPROPERTY(EditDefaultsOnly, Category=Body)
	TEnumAsByte<EItemEquipBodySlot::Type> ItemBodySlot;
};