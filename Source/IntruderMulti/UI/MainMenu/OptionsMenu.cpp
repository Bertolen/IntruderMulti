// Fill out your copyright notice in the Description page of Project Settings.

#include "OptionsMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"

UOptionsMenu::UOptionsMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerSettingsSave = "PlayerSettingsSave";
	WelcomeMessageVis = ESlateVisibility::Visible;
	AvatarCount = 0;
	AcceptIsEnabled = false;
}

void UOptionsMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Get the GameInstanceRef
	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()->GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstanceRef = GameInstance;

	// Check if there's a save file or if we should ask the player to make one
	SaveGameCheck();

	// Init the values
	EnteredPlayerName = FText::FromString(PlayerInfo.MyPlayerName);
	AvatarImage = PlayerInfo.MyPlayerImage;
	EmptyNameCheck(EnteredPlayerName);
}

void UOptionsMenu::SaveGameCheck()
{
	if (UGameplayStatics::DoesSaveGameExist(PlayerSettingsSave, 0)) {
		LoadGame();
		WelcomeMessageVis = ESlateVisibility::Hidden;
	}
	else {
		// Set a default avatar image
		if (AllAvatars.Num()) {
			PlayerInfo.MyPlayerImage = AllAvatars[0];
		}

		SaveGame();
		
		WelcomeMessageVis = ESlateVisibility::Visible;
	}
}

void UOptionsMenu::EmptyNameCheck(FText NameText)
{
	EnteredPlayerName = NameText;
	AcceptIsEnabled = !UKismetTextLibrary::TextIsEmpty(EnteredPlayerName);
}

void UOptionsMenu::LoadGame()
{
	UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerSettingsSave, 0));
	if (!PlayerSaveGame) {
		return;
	}

	PlayerInfo = PlayerSaveGame->S_PlayerInfo;
}

void UOptionsMenu::SaveGame()
{
	if (!SaveGameRef) {
		UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));
		if (!PlayerSaveGame) {
			return;
		}

		SaveGameRef = PlayerSaveGame;
	}

	SaveGameRef->S_PlayerInfo = PlayerInfo;
	UGameplayStatics::SaveGameToSlot(SaveGameRef, PlayerSettingsSave, 0);
}

void UOptionsMenu::ShowMainMenu()
{
	RemoveFromParent();

	GameInstanceRef->ShowMainMenu();
}

void UOptionsMenu::SaveOptions()
{
	// Saves the game
	PlayerInfo.MyPlayerName = EnteredPlayerName.ToString();
	PlayerInfo.MyPlayerImage = AvatarImage;
	SaveGame();

	// Record on the game instance that there's a save file now
	GameInstanceRef->CreatedSaveFile = true;

	// Hides the welcome message for the next time
	WelcomeMessageVis = ESlateVisibility::Hidden;

	// Go Back to the Main Menu
	ShowMainMenu();
}

void UOptionsMenu::CycleToNextAvatarImage()
{
	AvatarCount = UKismetMathLibrary::Clamp(AvatarCount + 1, 0, AllAvatars.Num() - 1);

	AvatarImage = AllAvatars[AvatarCount];
}

void UOptionsMenu::CycleToPreviousAvatarImage()
{
	AvatarCount = UKismetMathLibrary::Clamp(AvatarCount - 1, 0, AllAvatars.Num() - 1);

	AvatarImage = AllAvatars[AvatarCount];
}