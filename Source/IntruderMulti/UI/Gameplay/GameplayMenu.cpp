// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayMenu.h"
#include "Kismet/GameplayStatics.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"
#include "IntruderMulti/UI/Gameplay/EndGameWindow.h"
#include "Components/Border.h"

void UGameplayMenu::LeaveGame()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("LeaveGame - Begin"));

	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()->GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstance->DestroySessionCaller(GetOwningPlayer());

	UGameplayStatics::OpenLevel(GetOwningPlayer()->GetWorld(), "MainMenu");

	UE_LOG(IntruderDebug, Verbose, TEXT("LeaveGame - End"));
}

void UGameplayMenu::Hide()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Hide - Begin"));

	if (MenuWB) {
		MenuWB->SetVisibility(ESlateVisibility::Hidden);
	}

	if (EndGameWindowWB) {
		EndGameWindowWB->SetVisibility(ESlateVisibility::Hidden);
	}

	// Hide the cursor and binds the camera to be controlled by mouse movement again
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->bShowMouseCursor = false;

	UE_LOG(IntruderDebug, Verbose, TEXT("Hide - End"));
}

void UGameplayMenu::ShowMenu()
{
	if (MenuWB) {
		MenuWB->SetVisibility(ESlateVisibility::Visible);
	}

	// The mouse movement will no longer control the camera
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(GetCachedWidget());
	InputMode.SetHideCursorDuringCapture(true);
	GetOwningPlayer()->SetInputMode(InputMode);

	// Centers the mouse position
	UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings();
	FIntPoint ScreenResolution = GameSettings->GetScreenResolution();
	GetOwningPlayer()->SetMouseLocation(ScreenResolution.X / 2, ScreenResolution.Y / 2);

	// Show the cursor
	GetOwningPlayer()->bShowMouseCursor = true;
}

void UGameplayMenu::ShowEndGameWindow(FString WinText)
{
	if (EndGameWindowWB) {
		EndGameWindowWB->SetVisibility(ESlateVisibility::Visible);
		EndGameWindowWB->SetWinText(WinText);
		EndGameWindowWB->SetUserFocus(GetOwningPlayer());
	}
}