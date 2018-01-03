// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IntruderMultiGameMode.h"
#include "IntruderMultiCharacter.h"
#include "Characters/FP_Characters/Guard.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameUserSettings.h"

AIntruderMultiGameMode::AIntruderMultiGameMode()
	: Super()
{
	// set default classes to none
	DefaultPawnClass = NULL;
	GameSessionClass = NULL;
	HUDClass = NULL;
}

void AIntruderMultiGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Sets the game to be displayed on a 1280x720 window.
	UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings();
	GameSettings->SetScreenResolution(FIntPoint(1280, 720));
	GameSettings->SetFullscreenMode(EWindowMode::Windowed);
	GameSettings->ApplyResolutionSettings(false);
}