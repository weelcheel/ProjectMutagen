#pragma once

#include "DFInventoryItem.generated.h"

UCLASS(Blueprintable)
class ADFInventoryItem : public AActor
{
	GENERATED_UCLASS_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=ItemName)
	FString ItemName;

public:

	UFUNCTION(BlueprintCallable, Category=ItemName)
	FString GetItemName();
};