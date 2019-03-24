// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayHUD.h"
#include "IntruderMulti/UI/Gameplay/EndGameWindow.h"
#include "IntruderMulti/UI/Gameplay/GameplayMenu.h"

void UGameplayHUD::NativeConstruct()
{
	Super::NativeConstruct();

	//retrieve the game instance object and init the displayed time
	GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance) {
		DisplayedRemainingTime = FText::FromString(FString::Printf(TEXT("%d:00"), GameInstance->GameTime));
	}

	// hide the widgets
	if (EndGameWindowWB) {
		EndGameWindowWB->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GameplayMenuWB) {
		GameplayMenuWB->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGameplayHUD::UpdateRemainingTime(float ServerPlayTime)
{
	float TimeLeft = 0.0f;

	if (GameInstance) {
		TimeLeft = GameInstance->GameTime * 60 - ServerPlayTime;
	}

	int RemainingMinutes = TimeLeft / 60;
	int RemainingSeconds = TimeLeft - (RemainingMinutes * 60);

	DisplayedRemainingTime = FText::FromString(FString::Printf(TEXT("%d:%d"), RemainingMinutes, RemainingSeconds));
}

void UGameplayHUD::ShowEndGameWindow(FString WinText)
{
	if (EndGameWindowWB) {
		EndGameWindowWB->SetVisibility(ESlateVisibility::Visible);
		EndGameWindowWB->SetWinText(WinText);
		EndGameWindowWB->SetUserFocus(GetOwningPlayer());
	}
}

void UGameplayHUD::ShowMenuWindow()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UGameplayHUD.ShowMenuWindow - Begin"));

	if (GameplayMenuWB) {
		GameplayMenuWB->ShowMenu();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("UGameplayHUD.ShowMenuWindow - End"));
}