// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IntruderMultiGameMode.h"
#include "IntruderMultiHUD.h"
#include "IntruderMultiCharacter.h"
#include "UObject/ConstructorHelpers.h"

AIntruderMultiGameMode::AIntruderMultiGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AIntruderMultiHUD::StaticClass();
}
