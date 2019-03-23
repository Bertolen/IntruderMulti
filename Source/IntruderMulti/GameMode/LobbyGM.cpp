// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGM.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"
#include "IntruderMulti/GameState/LobbyGS.h"
#include "IntruderMulti/PlayerController/LobbyPC.h"
#include "IntruderMulti/UI/Lobby/LobbyMenu.h"

ALobbyGM::ALobbyGM(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{
	bReplicates = true;
	bUseSeamlessTravel = true;

	PlayerControllerClass = ALobbyPC::StaticClass();
}

void ALobbyGM::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGM, AllPlayerControllers);
	DOREPLIFETIME(ALobbyGM, CurrentPlayers);
	DOREPLIFETIME(ALobbyGM, ConnectedPlayers);
	DOREPLIFETIME(ALobbyGM, Characters);
}

void ALobbyGM::PostLogin(APlayerController * NewPlayer)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("PostLogin - Begin"));
	Super::PostLogin(NewPlayer);

	if (!HasAuthority()) { // Authority Only
		return;
	}

	
	// Update the Players list
	AllPlayerControllers.Add(NewPlayer);

	// Get the possible Spawn Locations
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnLocations);

	// Get some basic intel from the game instance
	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) {
		return;
	}
	
	ServerName = GameInstance->ServerName;
	MaxPlayers = GameInstance->MaxPlayers;

	ALobbyPC* lobbyPC = Cast<ALobbyPC>(NewPlayer);
	if (!lobbyPC) {
		return;
	}
	
	// init the lobby PC widgets
	lobbyPC->InitialSetup(ServerName);
	lobbyPC->SetupLobbyMenu(ServerName);

	RespawnPlayer(NewPlayer);
	
	UE_LOG(IntruderDebug, Verbose, TEXT("PostLogin - End"));
}

bool ALobbyGM::RespawnPlayer_Validate(APlayerController* PlayerController)
{
	return true;
}

void ALobbyGM::RespawnPlayer_Implementation(APlayerController* PlayerController)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("RespawnPlayer_Implementation - Begin"));
	if (PlayerController->GetCharacter()) {
		PlayerController->GetCharacter()->Destroy();
	}

	// Spawn the new character at a random spawn point
	int RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, SpawnLocations.Num() - 1);
	FTransform SpawnTransform = SpawnLocations[RandomIndex]->GetActorTransform();
	ACharacter * NewCharacter = GetWorld()->SpawnActor<ACharacter>(DefaultPawnClass, SpawnTransform);

	// Posses and updates the other clients widgets
	PlayerController->Possess(NewCharacter);
	EveryoneUpdate();
	UE_LOG(IntruderDebug, Verbose, TEXT("RespawnPlayer_Implementation - End"));
}

bool ALobbyGM::EveryoneUpdate_Validate()
{
	return true;
}

void ALobbyGM::EveryoneUpdate_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("EveryoneUpdate_Implementation - Begin"));
	CurrentPlayers = AllPlayerControllers.Num();

	if (CurrentPlayers <= 0) {
		return;
	}

	ALobbyGS* LobbyGS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!LobbyGS) {
		return;
	}

	// Updates the ConnectedPlayers array and checks if the game can start
	ConnectedPlayers.Empty();
	LobbyGS->SetCanWeStart(true);
	for (int i = 0; i < AllPlayerControllers.Num() ; i++)
	{
		ALobbyPC * LobbyPC = Cast<ALobbyPC>(AllPlayerControllers[i]);
		if (LobbyPC) {
			ConnectedPlayers.Add(LobbyPC->PlayerSettings);
			LobbyPC->UpdateNumberOfPlayers(CurrentPlayers, MaxPlayers);

			if (LobbyPC->PlayerSettings.MyPlayerStatus == EPlayerStatus::NotReady) { // if at least one player is not ready then we can't start the game
				LobbyGS->SetCanWeStart(false);
			} 
			else if (LobbyPC->PlayerSettings.MyPlayerStatus == EPlayerStatus::Host) { // If the host has yet to select a character then we can't start the game
				if (!LobbyPC->PlayerSettings.MyPlayerCharacter || LobbyPC->PlayerSettings.MyPlayerCharacter == DefaultPawnClass) {
					LobbyGS->SetCanWeStart(false);
				}
			}
		}
	}

	// Transfers the up-to-date ConnectedPlayers array to each player
	for (int i = 0; i < AllPlayerControllers.Num(); i++)
	{
		ALobbyPC * LobbyPC = Cast<ALobbyPC>(AllPlayerControllers[i]);
		if (LobbyPC) {
			LobbyPC->AddPlayerInfo(ConnectedPlayers);
			LobbyPC->UpdateTakenCharacters();
			AddToKickList();
		}
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("EveryoneUpdate_Implementation - End"));
}

bool ALobbyGM::ServerUpdateGameSettings_Validate(int MapID, int TimeID)
{
	return true;
}

void ALobbyGM::ServerUpdateGameSettings_Implementation(int MapID, int TimeID)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ServerUpdateGameSettings_Implementation - Begin"));

	ALobbyGS* LobbyGS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!LobbyGS) {
		return;
	}
	LobbyGS->SetTimeID(TimeID);
	LobbyGS->SetMapID(MapID);

	for (int i = 0; i < AllPlayerControllers.Num(); i++)
	{
		ALobbyPC * lobbyPC = Cast<ALobbyPC>(AllPlayerControllers[i]);
		if (lobbyPC) {
			lobbyPC->UpdateLobbySettings();
		}
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("ServerUpdateGameSettings_Implementation - End"));
}

void ALobbyGM::LaunchTheGame()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("LaunchTheGame - Begin"));
	ALobbyGS* LobbyGS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!LobbyGS) {
		return;
	}

	switch (LobbyGS->GetMapID())
	{
	case 0:
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), FString("servertravel /Game/IntruderMulti/Maps/Arena01"), UGameplayStatics::GetPlayerController(GetWorld(), 0));;
		break;
	case 1:
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), FString("servertravel /Game/IntruderMulti/Maps/Consulate"), UGameplayStatics::GetPlayerController(GetWorld(), 0));
		break;
	default:
		break;
	}
	UE_LOG(IntruderDebug, Verbose, TEXT("LaunchTheGame - End"));
}

bool ALobbyGM::YouHaveBeenKicked_Validate(int PlayerID)
{
	return true;
}

void ALobbyGM::YouHaveBeenKicked_Implementation(int PlayerID)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("YouHaveBeenKicked_Implementation - Begin"));
	ALobbyPC* lobbyPC = Cast<ALobbyPC>(AllPlayerControllers[PlayerID]);

	if (lobbyPC) {
		lobbyPC->Kicked();
	}
	UE_LOG(IntruderDebug, Verbose, TEXT("YouHaveBeenKicked_Implementation - End"));
}

void ALobbyGM::Logout(AController* Exiting)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Logout - Begin"));

	Super::Logout(Exiting);

	int i;
	for (i = 0; i < AllPlayerControllers.Num(); i++)
	{
		if (Exiting == AllPlayerControllers[i]) {
			break;
		}
	}

	ALobbyPC * LobbyPC = Cast<ALobbyPC>(AllPlayerControllers[i]);
	if (!LobbyPC) {
		return;
	}

	ALobbyGS * LobbyGS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!LobbyGS) {
		return;
	}

	// Frees the character that was taken
	LobbyGS->SetTakenCharacterByIndex(LobbyPC->SelectedCharacter, false);

	AllPlayerControllers.RemoveAt(i);
	ConnectedPlayers.RemoveAt(i);

	EveryoneUpdate();

	UE_LOG(IntruderDebug, Verbose, TEXT("Logout - End"));
}

void ALobbyGM::AddToKickList()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AddToKickList - Begin"));
	ALobbyPC* LobbyPC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!LobbyPC) {
		return;
	}
	
	if (LobbyPC->LobbyMenuWB) {
		if (!LobbyPC->LobbyMenuWB->GetGameSettingsWB()) {
			UE_LOG(IntruderDebug, Error, TEXT("No Game Settings Widget found !!"));
			return;
		}
		if (LobbyPC->LobbyMenuWB->GetGameSettingsWB()->IsVisible()) {
			LobbyPC->LobbyMenuWB->GetGameSettingsWB()->FillPlayersWindow();
		}
	}
	UE_LOG(IntruderDebug, Verbose, TEXT("AddToKickList - End"));
}

bool ALobbyGM::SwapCharacters_Validate(APlayerController* PlayerController, TSubclassOf<ACharacter> CharacterClass, bool ChangedStatus)
{
	return true;
}

void ALobbyGM::SwapCharacters_Implementation(APlayerController* PlayerController, TSubclassOf<ACharacter> CharacterClass, bool ChangedStatus)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SwapCharacters_Implementation - Begin"));
	if (ChangedStatus) {
		return;
	}

	if (PlayerController->GetCharacter()) {
		PlayerController->GetCharacter()->Destroy();
	}

	// Spawn the new character at a random spawn point
	int RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, SpawnLocations.Num() - 1);
	FTransform SpawnTransform = SpawnLocations[RandomIndex]->GetActorTransform();
	ACharacter * NewCharacter = GetWorld()->SpawnActor<ACharacter>(CharacterClass, SpawnTransform);

	// Posses and updates the other clients widgets
	PlayerController->Possess(NewCharacter);
	UE_LOG(IntruderDebug, Verbose, TEXT("SwapCharacters_Implementation - End"));
}