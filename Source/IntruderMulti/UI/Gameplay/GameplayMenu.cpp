// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayMenu.h"
#include "Kismet/GameplayStatics.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"

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

	SetVisibility(ESlateVisibility::Hidden);

	// Hide the cursor and binds the camera to be controlled by mouse movement again
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->bShowMouseCursor = false;

	UE_LOG(IntruderDebug, Verbose, TEXT("Hide - End"));
}