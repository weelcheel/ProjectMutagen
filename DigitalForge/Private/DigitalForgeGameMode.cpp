// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "DigitalForge.h"
#include "DigitalForgeGameMode.h"
#include "DigitalForgeCharacter.h"
#include "GameFramework/HUD.h"
#include "DFPlayerController.h"

ADigitalForgeGameMode::ADigitalForgeGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/DEMOCharacters/DFDemoPlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDBPClass(TEXT("/Game/Blueprints/HUD/DFDemoHUD"));
	if (PlayerHUDBPClass.Class != NULL)
	{
		HUDClass = PlayerHUDBPClass.Class;
	}

	PlayerControllerClass = ADFPlayerController::StaticClass();
}
