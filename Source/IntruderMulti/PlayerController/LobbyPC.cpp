// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPC.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"
#include "IntruderMulti/GameMode/LobbyGM.h"
#include "IntruderMulti/GameState/LobbyGS.h"
#include "IntruderMulti/UI/AllLevels/LoadingScreen.h"
#include "IntruderMulti/UI/Lobby/LobbyMenu.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"

ALobbyPC::ALobbyPC(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{
	bReplicates = true;

	PlayerSettingsSave = "PlayerSettingsSave";
}

void ALobbyPC::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPC, PlayerSettings);
	DOREPLIFETIME(ALobbyPC, AllConnectedPlayers);
	DOREPLIFETIME(ALobbyPC, SelectedCharacter);
	DOREPLIFETIME(ALobbyPC, PreviousSelection);
	DOREPLIFETIME(ALobbyPC, CharacterImage);
	DOREPLIFETIME(ALobbyPC, SenderText);
	DOREPLIFETIME(ALobbyPC, SenderName);
}

void ALobbyPC::SetupInputComponent()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SetupInputComponent - Begin"));
	Super::SetupInputComponent();

	InputComponent->BindAction("ToggleDisplayMenus", IE_Released, this, &ALobbyPC::ToggleDisplay);
	UE_LOG(IntruderDebug, Verbose, TEXT("SetupInputComponent - End"));
}

void ALobbyPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("EndPlay - Begin"));
	
	Super::EndPlay(EndPlayReason);

	// Clears the timers (if any)
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	// Destroys the session
	UGameInfoInstance * GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstance->DestroySessionCaller(this);
	UE_LOG(IntruderDebug, Verbose, TEXT("EndPlay - End"));
}

void ALobbyPC::AssignPlayer(TSubclassOf<ACharacter> Character, UTexture2D * CharacterImage)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AssignPlayer - Begin"));

	PlayerSettings.MyPlayerCharacter = Character;
	PlayerSettings.MyPlayerCharacterImage = CharacterImage;

	CallUpdate(PlayerSettings, false);

	UE_LOG(IntruderDebug, Verbose, TEXT("AssignPlayer - End"));
}

void ALobbyPC::SaveGameCheck()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SaveGameCheck - Begin"));

	if (UGameplayStatics::DoesSaveGameExist(PlayerSettingsSave, 0)) {
		LoadGame();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("SaveGameCheck - End"));
}

void ALobbyPC::LoadGame()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("LoadGame - Begin"));

	UPlayerSaveGame* SaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerSettingsSave, 0));
	if (!SaveGame) {
		UE_LOG(IntruderDebug, Error, TEXT("Load failed! Player won't be correctly loaded."));
		return;
	}

	PlayerSettings.MyPlayerName = SaveGame->S_PlayerInfo.MyPlayerName;
	PlayerSettings.MyPlayerImage = SaveGame->S_PlayerInfo.MyPlayerImage;

	UE_LOG(IntruderDebug, Verbose, TEXT("Loaded save game for player %s"), *PlayerSettings.MyPlayerName);

	UE_LOG(IntruderDebug, Verbose, TEXT("LoadGame - End"));
}

void ALobbyPC::ToggleDisplay()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ToggleDisplay - Begin"));

	LobbyMenuWB->ToggleDisplay();

	UE_LOG(IntruderDebug, Verbose, TEXT("ToggleDisplay - End"));
}

////////////// Clients Functions

void ALobbyPC::InitialSetup_Implementation(const FText & ServerName)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("InitialSetup_Implementation - Begin"));

	SaveGameCheck();
	CallUpdate(PlayerSettings, true);

	UE_LOG(IntruderDebug, Verbose, TEXT("InitialSetup_Implementation - End"));
}

void ALobbyPC::SetupLobbyMenu_Implementation(const FText & ServerName)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SetupLobbyMenu_Implementation - Begin"));

	bShowMouseCursor = true;

	if (LobbyMenuClass != nullptr) { // check if our widget class exists, else we'll crash
		if (LobbyMenuWB == nullptr) { // init the widget
			LobbyMenuWB = CreateWidget<ULobbyMenu>(GetWorld(), LobbyMenuClass);
		}

		UpdateLobbySettings();

		LobbyMenuWB->LobbyServerName = ServerName;
		LobbyMenuWB->AddToViewport();

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(LobbyMenuWB->GetCachedWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(true);

		SetInputMode(InputMode);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("SetupLobbyMenu_Implementation - End"));
}

void ALobbyPC::AddPlayerInfo_Implementation(const TArray<FPlayerInfo> & ConnectedPlayersInfo)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AddPlayerInfo_Implementation - Begin"));
	
	AllConnectedPlayers = ConnectedPlayersInfo;
	
	if (!LobbyMenuWB) {
		return;
	}
	
	LobbyMenuWB->ClearPlayerList();

	for (int i = 0; i < AllConnectedPlayers.Num(); i++)
	{
		LobbyMenuWB->UpdatePlayerWindow(AllConnectedPlayers[i]);
	}
	
	UE_LOG(IntruderDebug, Verbose, TEXT("AddPlayerInfo_Implementation - End"));
}

void ALobbyPC::UpdateLobbySettings_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateLobbySettings_Implementation - Begin"));

	ALobbyGS * LobbyGS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!LobbyGS) {
		GetWorld()->GetTimerManager().SetTimer(InitLobbySettingsTimerHandle, this, &ALobbyPC::UpdateLobbySettings, 0.5f);
		return;
	}

	LobbyMenuWB->MapImage = LobbyGS->GetMapImage();
	LobbyMenuWB->MapName = LobbyGS->GetMapName();
	LobbyMenuWB->MapTime = LobbyGS->GetTime();

	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateLobbySettings_Implementation - End"));
}

void ALobbyPC::ShowLoadingScreen_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ShowLoadingScreen_Implementation - Begin"));

	LobbyMenuWB->RemoveFromParent();

	if (LoadingScreenClass != nullptr) {
		// Create a loading screen and displays it
		CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass)->AddToViewport();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("ShowLoadingScreen_Implementation - End"));
}

void ALobbyPC::UpdateNumberOfPlayers_Implementation(int CurrentPlayers, int MaxPlayers)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateNumberOfPlayers_Implementation - Begin"));

	if (!LobbyMenuWB) {
		return;
	}

	LobbyMenuWB->PlayersDisplay = FText::FromString(FString::FromInt(CurrentPlayers) + " of " + FString::FromInt(MaxPlayers));

	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateNumberOfPlayers_Implementation - End"));
}

void ALobbyPC::Kicked_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Kicked_Implementation - Begin"));

	UGameInfoInstance * GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstance->DestroySessionCaller(this);

	UE_LOG(IntruderDebug, Verbose, TEXT("Kicked_Implementation - End"));
}

void ALobbyPC::UpdateTakenCharacters_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateTakenCharacters_Implementation - Begin"));
	
	if (!LobbyMenuWB) {
		return;
	}

	LobbyMenuWB->GetCharacterSelectWB()->UpdateEnabledButtons();

	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateTakenCharacters_Implementation - End"));
}

void ALobbyPC::UpdateChat_Implementation(const FText & Sender, const FText & Text)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateChat_Implementation - Begin"));

	if (!LobbyMenuWB) {
		return;
	}

	LobbyMenuWB->GetChatWindowWB()->UpdateChatWindow(Sender, Text);

	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateChat_Implementation - End"));
}

////////////// Server Functions

bool ALobbyPC::CallUpdate_Validate(FPlayerInfo PlayerInfo, bool ChangedStatus)
{
	return true;
}

void ALobbyPC::CallUpdate_Implementation(FPlayerInfo PlayerInfo, bool ChangedStatus)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("CallUpdate_Implementation - Begin"));

	PlayerSettings = PlayerInfo;

	ALobbyGM * LobbyGM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!LobbyGM) {
		return;
	}

	LobbyGM->SwapCharacters(this, PlayerSettings.MyPlayerCharacter, ChangedStatus);
	LobbyGM->EveryoneUpdate();

	UE_LOG(IntruderDebug, Verbose, TEXT("CallUpdate_Implementation - End"));
}

bool ALobbyPC::AssignSelectedCharacter_Validate(int CharacterID)
{
	return true;
}

void ALobbyPC::AssignSelectedCharacter_Implementation(int CharacterID)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AssignSelectedCharacter_Implementation - Begin"));

	ALobbyGS* LobbyGS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!LobbyGS) {
		return;
	}

	if (CharacterID >= LobbyGS->GetAllCharacterImages().Num()) {
		UE_LOG(IntruderDebug, Warning, TEXT("Attempted to get a character with an invalid index. Request ignored."));
		return;
	}

	PreviousSelection = SelectedCharacter;
	SelectedCharacter = CharacterID;
	CharacterImage = LobbyGS->GetAllCharacterImages()[SelectedCharacter];

	CharacterCheck();

	UE_LOG(IntruderDebug, Verbose, TEXT("AssignSelectedCharacter_Implementation - End"));
}

bool ALobbyPC::GetChatMessage_Validate(const FText & TextToSend)
{
	return true;
}

void ALobbyPC::GetChatMessage_Implementation(const FText & TextToSend)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("GetChatMessage_Implementation - Begin"));

	SenderText = TextToSend;
	SenderName = FText::FromString(PlayerSettings.MyPlayerName);

	ALobbyGM * LobbyGM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!LobbyGM) {
		return;
	}

	for (int i = 0; i < LobbyGM->AllPlayerControllers.Num(); i++)
	{
		ALobbyPC* LobbyPC = Cast<ALobbyPC>(LobbyGM->AllPlayerControllers[i]);
		if(!LobbyPC) {
			return;
		}

		LobbyPC->UpdateChat(SenderName, SenderText);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("GetChatMessage_Implementation - End"));
}

bool ALobbyPC::CharacterCheck_Validate()
{
	return true;
}

void ALobbyPC::CharacterCheck_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("CharacterCheck - Begin"));

	ALobbyGS * LobbyGS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!LobbyGS) {
		return;
	}

	// Takes the new character (the base character is an exception, he's always free so never taken)
	if (SelectedCharacter != 0) {
		if (LobbyGS->GetTakenCharacterByIndex(SelectedCharacter) == false) {
			LobbyGS->SetTakenCharacterByIndex(SelectedCharacter, true);
		}
	}

	// Frees the previous character
	LobbyGS->SetTakenCharacterByIndex(PreviousSelection, false);

	// Assigns the character to the player
	ALobbyGM * LobbyGM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!LobbyGM) {
		return;
	}

	AssignPlayer(LobbyGM->Characters[SelectedCharacter], CharacterImage);

	UE_LOG(IntruderDebug, Verbose, TEXT("CharacterCheck - End"));
}