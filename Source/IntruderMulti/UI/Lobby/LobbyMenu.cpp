// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyMenu.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IntruderMulti/PlayerController/LobbyPC.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"
#include "IntruderMulti/GameMode/LobbyGM.h"

void ULobbyMenu::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULobbyMenu, LobbyServerName);
	DOREPLIFETIME(ULobbyMenu, MapName);
	DOREPLIFETIME(ULobbyMenu, MapTime);
	DOREPLIFETIME(ULobbyMenu, MapImage);
	DOREPLIFETIME(ULobbyMenu, PlayersDisplay);
	DOREPLIFETIME(ULobbyMenu, ReadyButtonText);
	DOREPLIFETIME(ULobbyMenu, ReadyStatus);
	DOREPLIFETIME(ULobbyMenu, DefaultPawnClass);
}

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(IntruderDebug, Verbose, TEXT("NativeConstruct - Begin"));

	ALobbyPC* LobbyPC = Cast<ALobbyPC>(GetOwningPlayer());
	if (!LobbyPC) {
		return;
	}

	LobbyPcRef = LobbyPC;

	if (UKismetSystemLibrary::IsServer(GetOwningPlayer()->GetWorld())) {
		ReadyButtonText = FText::FromString("Start Session");
		LobbyPcRef->PlayerSettings.MyPlayerStatus = EPlayerStatus::Host;
	}
	else {
		ReadyButtonText = FText::FromString("Toggle Ready");
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("NativeConstruct - End"));
}

void ULobbyMenu::LeaveLobby()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("LeaveLobby - Begin"));
	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()->GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstance->DestroySessionCaller(GetOwningPlayer());
	UGameplayStatics::OpenLevel(GetOwningPlayer()->GetWorld(), "MainMenu");
	UE_LOG(IntruderDebug, Verbose, TEXT("LeaveLobby - End"));
}

void ULobbyMenu::ToggleStatus()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ToggleStatus - Begin"));
	
	//Change the status text
	if (ReadyStatus == EPlayerStatus::Ready) {
		SetReadyStatus(EPlayerStatus::NotReady);
	}
	else {
		SetReadyStatus(EPlayerStatus::Ready);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("ToggleStatus - End"));
}

void ULobbyMenu::LaunchTheGame()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("LaunchTheGame - Begin"));

	ALobbyGM* LobbyGM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetOwningPlayer()->GetWorld()));
	if (!LobbyGM) {
		UE_LOG(IntruderDebug, Error, TEXT("No GameMode found to launch the game. Launch canceled."));
		return;
	}

	// Show the loading screen to all the players
	for (int i = 0; i < LobbyGM->AllPlayerControllers.Num(); i++)
	{
		ALobbyPC* LobbyPC = Cast<ALobbyPC>(LobbyGM->AllPlayerControllers[i]);
		if (!LobbyPC) {
			UE_LOG(IntruderDebug, Error, TEXT("Could not cast player controller. Won't be able to show load loading screen."));
		}
		else {
			LobbyPC->ShowLoadingScreen();
		}
	}

	//Launch the game
	LobbyGM->LaunchTheGame();
	UE_LOG(IntruderDebug, Verbose, TEXT("LaunchTheGame - End"));
}

bool ULobbyMenu::EnabledReadyStartButton()
{
	if (UKismetSystemLibrary::IsServer(GetOwningPlayer()->GetWorld())) {
		ALobbyGS* LobbyGS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetOwningPlayer()->GetWorld()));
		if (!LobbyGS) {
			return false;
		}
		return LobbyGS->GetCanWeStart();
	}
	else {
		ALobbyPC* LobbyPC = Cast<ALobbyPC>(GetOwningPlayer());
		if (!LobbyPC) {
			return false;
		}

		if (!LobbyPC->PlayerSettings.MyPlayerCharacter || LobbyPC->PlayerSettings.MyPlayerCharacter == DefaultPawnClass) {
			return false;
		}
	}
	return true;
}

void ULobbyMenu::ShowGameSettingsWB()
{
	if (!GameSettingsWB) {
		return;
	}

	if (!GameSettingsWB->IsVisible()) {
		GameSettingsWB->FillPlayersWindow();
		GameSettingsWB->SetVisibility(ESlateVisibility::Visible);
		GameSettingsWB->SetUserFocus(GetOwningPlayer());
	}
}

void ULobbyMenu::ShowCharacterSelectWB()
{
	if (!CharacterSelectWB) {
		return;
	}

	if (!CharacterSelectWB->IsVisible()) {
		CharacterSelectWB->UpdateEnabledButtons();
		CharacterSelectWB->SetVisibility(ESlateVisibility::Visible);
		CharacterSelectWB->SetUserFocus(GetOwningPlayer());
	}
}

void ULobbyMenu::UpdatePlayerWindow_Implementation(FPlayerInfo IncomingPlayerInfo)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdatePlayerWindow - Begin"));
	UConnectedPlayer * PlayerWidget = CreateWidget<UConnectedPlayer>(GetOwningPlayer()->GetWorld(), ConnectedPlayerClass);
	if (!PlayerWidget) {
		return;
	}

	PlayerWidget->SetPlayerInfo(IncomingPlayerInfo);
	AddPlayerToList(PlayerWidget);
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdatePlayerWindow - End"));
}

////// Setters
void ULobbyMenu::SetReadyStatus(const EPlayerStatus & NewStatus)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SetReadyStatus - Begin"));

	ReadyStatus = NewStatus;

	LobbyPcRef->PlayerSettings.MyPlayerStatus = ReadyStatus;
	LobbyPcRef->CallUpdate(LobbyPcRef->PlayerSettings, true);

	UE_LOG(IntruderDebug, Verbose, TEXT("SetReadyStatus - End"));
}