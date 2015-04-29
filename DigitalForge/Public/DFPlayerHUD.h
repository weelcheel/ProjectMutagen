#pragma once

#include "GameFramework/HUD.h"
#include "DFPlayerHUD.generated.h"

UCLASS(Blueprintable)
class ADFPlayerHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

protected:

	UFUNCTION(BlueprintImplementableEvent, Category=HUD)
	virtual void PostRender();
};