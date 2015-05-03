#pragma once

#include "DFInventoryItem.generated.h"

class ADigitalForgeCharacter;

UCLASS()
class ADFInventoryItem : public AActor
{
	GENERATED_UCLASS_BODY()

	/** [server] weapon was added to pawn's inventory */
	virtual void OnEnterInventory(ADigitalForgeCharacter* NewOwner);

	/** [server] weapon was removed from pawn's inventory */
	virtual void OnLeaveInventory();

	/** if this item can be used by the owner */
	UPROPERTY(EditDefaultsOnly, Category=Item)
	bool bConsumableItem;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=ItemName)
	FString ItemName;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_MyPawn)
	class ADigitalForgeCharacter* MyPawn;

	UFUNCTION()
	void OnRep_MyPawn();

public:

	/** set the inventory's owning pawn */
	void SetOwningPawn(ADigitalForgeCharacter* DFCharacter);

	UFUNCTION(BlueprintCallable, Category=ItemName)
	FString GetItemName();

	/** get pawn owner */
	UFUNCTION(BlueprintCallable, Category="Game")
	class ADigitalForgeCharacter* GetPawnOwner() const;
};