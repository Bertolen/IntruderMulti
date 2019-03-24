// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayPC.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "IntruderMulti/GameMode/GameplayGM.h"
#include "IntruderMulti/UI/Gameplay/GameplayMenu.h"
#include "IntruderMulti/UI/Gameplay/GameplayHUD.h"
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
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.BeginPlay - Begin"));

	Super::BeginPlay();

	if (IsLocalController()) {
		// clean up the display
		UWidgetLayoutLibrary::RemoveAllWidgets(this);

		// init the character
		PassCharacterInfoToServer();

		// Set up the input mode (hide cursor and all)
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		// Display the HUD
		if (GameplayHUDClass != nullptr) { // check if our widget class exists, else we'll crash
			if (GameplayHUDWB == nullptr) { // init the widget
				GameplayHUDWB = CreateWidget<UGameplayHUD>(GetWorld(), GameplayHUDClass);
				GameplayHUDWB->AddToViewport();
			}

			// Show our widget
			GameplayHUDWB->SetVisibility(ESlateVisibility::Visible);
		}
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.BeginPlay - End"));
}

void AGameplayPC::TypeChatMessage()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.TypeChatMessage - Begin"));

	if (GameplayHUDWB && GameplayHUDWB->GetChatWindow()) {
		GameplayHUDWB->GetChatWindow()->StartTyping(true);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.TypeChatMessage - End"));
}

bool AGameplayPC::PassCharacterInfoToServer_Validate()
{
	return true;
}

void AGameplayPC::PassCharacterInfoToServer_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.PassCharacterInfoToServer_Implementation - Begin"));

	SenderName = FText::FromString(PlayerSettings.MyPlayerName);

	if (!PlayerSettings.MyPlayerCharacter) {
		UE_LOG(IntruderDebug, Warning, TEXT("Player character null. Spawn will use the default pawn class."));
	}

	AGameplayGM * GameplayGM = Cast<AGameplayGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameplayGM) {
		return;
	}

	GameplayGM->RespawnPlayer(this, PlayerSettings.MyPlayerCharacter);

	SetupHUD();

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.PassCharacterInfoToServer_Implementation - End"));
}

bool AGameplayPC::GetChatMessage_Validate(const FText & Text)
{
	return true;
}

void AGameplayPC::GetChatMessage_Implementation(const FText &  Text)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.GetChatMessage_Implementation - Begin"));

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

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.GetChatMessage_Implementation - End"));
}

void AGameplayPC::SetupHUD_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.SetupHUD_Implementation - Begin"));

	if (GameplayHUDClass != nullptr) { // check if our widget class exists, else we'll crash
		if (GameplayHUDWB == nullptr) { // init the widget
			GameplayHUDWB = CreateWidget<UGameplayHUD>(GetWorld(), GameplayHUDClass);
			GameplayHUDWB->AddToViewport();
		}
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.SetupHUD_Implementation - End"));
}

void AGameplayPC::ShowMenuWindow()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.ShowMenuWindow - Begin"));

	if (GameplayHUDWB) { 
		GameplayHUDWB->ShowMenuWindow();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.ShowMenuWindow - End"));
}

void AGameplayPC::UpdateChat_Implementation(const FText & _SenderName, const FText & _SenderText)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.UpdateChat_Implementation - Begin"));

	if (GameplayHUDWB && GameplayHUDWB->GetChatWindow()) {
		GameplayHUDWB->GetChatWindow()->UpdateChatWindow(_SenderName, _SenderText);
	}
	else {
		UE_LOG(IntruderDebug, Warning, TEXT("Gameplay Chat Window not found. Messages won't appear on screen."));
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.UpdateChat_Implementation - End"));
}

bool AGameplayPC::InitFromLobbyPC_Validate(ALobbyPC* LobbyPC)
{
	return true;
}

void AGameplayPC::InitFromLobbyPC_Implementation(ALobbyPC* LobbyPC)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.InitFromLobbyPC - Begin"));

	PlayerSettings = LobbyPC->PlayerSettings;

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.InitFromLobbyPC - End"));
}

void AGameplayPC::DisplayEndGameWidget_Implementation(const FString & WinText)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.DisplayEndGameWidget_Implementation - Begin"));

	GameplayHUDWB->ShowEndGameWindow(WinText);

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(GameplayHUDWB->GetCachedWidget());
	SetInputMode(InputMode);

	bShowMouseCursor = true;

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.DisplayEndGameWidget_Implementation - End"));
}

void AGameplayPC::PassPlayTime_Implementation(const float ServerPlayTime)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.PassRemainingTime_Implementation - Begin"));

	if (GameplayHUDWB) { 
		GameplayHUDWB->UpdateRemainingTime(ServerPlayTime);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayPC.PassRemainingTime_Implementation - Begin"));
}