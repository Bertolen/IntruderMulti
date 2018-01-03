// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSelect.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"
#include "IntruderMulti/PlayerController/LobbyPC.h"
#include "IntruderMulti/UI/Lobby/LobbyMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UCharacterSelect::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()->GetWorld()));
	if (GameInstance) {
		GameInstanceRef = GameInstance;
	}
}


void UCharacterSelect::SelectCharacterByID(int CharacterID)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SelectCharacterByID - Begin"));

	ALobbyPC* LobbyPC = Cast<ALobbyPC>(GetOwningPlayer());
	if (!LobbyPC) {
		return;
	}

	// Everytime the player changes character his/her status becomes not ready, unless he's the host
	if (LobbyPC->PlayerSettings.MyPlayerStatus != EPlayerStatus::Host) {
		LobbyPC->LobbyMenuWB->SetReadyStatus(EPlayerStatus::NotReady); 
	}

	// Assign the selected character
	LobbyPC->AssignSelectedCharacter(CharacterID);

	// hide the window after selection
	SetVisibility(ESlateVisibility::Hidden);

	UE_LOG(IntruderDebug, Verbose, TEXT("SelectCharacterByID - End"));
}

void UCharacterSelect::UpdateEnabledButtons(TArray<bool> TakenCharacters)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateEnabledButtons - Begin"));

	if (TakenCharacters.Num() != CharacterButtons.Num()) {
		UE_LOG(IntruderDebug, Warning, TEXT("There's not the same amount of characters and of buttons. All buttons active by default."));
		return;
	}

	for (int i = 0; i < CharacterButtons.Num(); i++)
	{
		CharacterButtons[i]->SetIsEnabled(!TakenCharacters[i]);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateEnabledButtons - End"));
}