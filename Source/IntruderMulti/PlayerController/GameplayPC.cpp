// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayPC.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "IntruderMulti/GameMode/GameplayGM.h"
#include "IntruderMulti/UI/Gameplay/GameplayMenu.h"
#include "IntruderMulti/UI/Gameplay/EndGameWindow.h"
#include "IntruderMulti/PlayerController/LobbyPC.h"

AGameplayPC::AGameplayPC(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerSettingsSave = "PlayerSettingsSave";
}

void AGameplayPC::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameplayPC, PlayerSettings);
	DOREPLIFETIME(AGameplayPC, SenderName);
}

void AGameplayPC::BeginPlay()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("BeginPlay - Begin"));

	Super::BeginPlay();

	if (IsLocalController()) {
		UWidgetLayoutLibrary::RemoveAllWidgets(this);

		PassCharacterInfoToServer(PlayerSettings);

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("BeginPlay - End"));
}

void AGameplayPC::ToggleDisplay()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ToggleDisplay - Begin"));

	bShowMouseCursor = !bShowMouseCursor;

	UE_LOG(IntruderDebug, Verbose, TEXT("ToggleDisplay - End"));
}

void AGameplayPC::TypeChatMessage()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("TypeChatMessage - Begin"));

	if (GameplayMenuWB && GameplayMenuWB->GetChatWindow()) {
		GameplayMenuWB->GetChatWindow()->StartTyping();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("TypeChatMessage - End"));
}

bool AGameplayPC::PassCharacterInfoToServer_Validate(FPlayerInfo PlayerSettingsInfo)
{
	return true;
}

void AGameplayPC::PassCharacterInfoToServer_Implementation(FPlayerInfo PlayerSettingsInfo)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("PassCharacterInfoToServer_Implementation - Begin"));

	PlayerSettings = PlayerSettingsInfo;
	SenderName = FText::FromString(PlayerSettings.MyPlayerName);

	if (!PlayerSettings.MyPlayerCharacter) {
		UE_LOG(IntruderDebug, Warning, TEXT("Player character null. Spawn will use the default pawn class."));
	}

	AGameplayGM * GameplayGM = Cast<AGameplayGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameplayGM) {
		return;
	}

	GameplayGM->RespawnPlayer(this, PlayerSettings.MyPlayerCharacter);

	SetupMenuWindow();

	UE_LOG(IntruderDebug, Verbose, TEXT("PassCharacterInfoToServer_Implementation - End"));
}

bool AGameplayPC::GetChatMessage_Validate(const FText & Text)
{
	return true;
}

void AGameplayPC::GetChatMessage_Implementation(const FText &  Text)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("GetChatMessage_Implementation - Begin"));

	SenderText = Text;

	AGameplayGM * GameplayGM = Cast<AGameplayGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameplayGM) {
		return;
	}

	for (int i = 0; i < GameplayGM->GetAllPlayerControllers().Num(); i++)
	{
		AGameplayPC* GameplayPC = Cast<AGameplayPC>(GameplayGM->GetAllPlayerControllers()[i]);
		if (!GameplayPC) {
			return;
		}

		GameplayPC->UpdateChat(SenderName, SenderText);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("GetChatMessage_Implementation - End"));
}

void AGameplayPC::SetupMenuWindow_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SetupChatWindow_Implementation - Begin"));

	// TODO : use the gameplay menu to display the gameplay chat
	if (GameplayMenuClass != nullptr) { // check if our widget class exists, else we'll crash
		if (GameplayMenuWB == nullptr) { // init the widget
			GameplayMenuWB = CreateWidget<UGameplayMenu>(GetWorld(), GameplayMenuClass);
		}
		GameplayMenuWB->AddToViewport();

		// hide the menu
		GameplayMenuWB->Hide();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("SetupChatWindow_Implementation - End"));
}

void AGameplayPC::ShowMenuWindow()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ShowMenuWindow - Begin"));

	if (GameplayMenuClass != nullptr) { // check if our widget class exists, else we'll crash
		if (GameplayMenuWB == nullptr) { // init the widget
			GameplayMenuWB = CreateWidget<UGameplayMenu>(GetWorld(), GameplayMenuClass);
			GameplayMenuWB->AddToViewport();
		}

		// Show our widget
		GameplayMenuWB->ShowMenu();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("ShowMenuWindow - End"));
}

void AGameplayPC::UpdateChat_Implementation(const FText &  SenderName, const FText &  SenderText)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateChat_Implementation - Begin"));

	if (GameplayMenuWB && GameplayMenuWB->GetChatWindow()) {
		GameplayMenuWB->GetChatWindow()->UpdateChatWindow(SenderName, SenderText);
	}
	else {
		UE_LOG(IntruderDebug, Warning, TEXT("Gameplay Chat Window not found. Messages won't appear on screen."));
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateChat_Implementation - End"));
}

bool AGameplayPC::InitFromLobbyPC_Validate(ALobbyPC* LobbyPC)
{
	return true;
}

void AGameplayPC::InitFromLobbyPC_Implementation(ALobbyPC* LobbyPC)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("InitFromLobbyPC - Begin"));

	PlayerSettings = LobbyPC->PlayerSettings;

	UE_LOG(IntruderDebug, Verbose, TEXT("InitFromLobbyPC - End"));
}

void AGameplayPC::DisplayEndGameWidget_Implementation(const FString & WinText)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("DisplayEndGameWidget_Implementation - Begin"));

	GameplayMenuWB->ShowEndGameWindow(WinText);

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(GameplayMenuWB->GetCachedWidget());
	SetInputMode(InputMode);

	bShowMouseCursor = true;

	UE_LOG(IntruderDebug, Verbose, TEXT("DisplayEndGameWidget_Implementation - End"));
}