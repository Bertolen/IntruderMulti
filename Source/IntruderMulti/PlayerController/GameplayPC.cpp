// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayPC.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "IntruderMulti/GameMode/GameplayGM.h"
#include "IntruderMulti/UI/AllLevels/GameplayChat.h"

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

void AGameplayPC::SetupInputComponent()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SetupInputComponent - Begin"));
	Super::SetupInputComponent();

	InputComponent->BindAction("ToggleDisplayMenus", IE_Released, this, &AGameplayPC::ToggleDisplay);
	UE_LOG(IntruderDebug, Verbose, TEXT("SetupInputComponent - End"));
}

void AGameplayPC::BeginPlay()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("BeginPlay - Begin"));

	Super::BeginPlay();

	if (IsLocalController()) {
		UWidgetLayoutLibrary::RemoveAllWidgets(this);
		LoadGame(); 

		if (!PlayerSettings.MyPlayerCharacter) {
			UE_LOG(IntruderDebug, Error, TEXT("Player character null. Spawn WILL fail."));
		}

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

void AGameplayPC::LoadGame()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("LoadGame - Begin"));
	
	UPlayerSaveGame* SaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerSettingsSave, 0));
	if (!SaveGame) {
		return;
	}

	PlayerSettings = SaveGame->S_PlayerInfo;

	UE_LOG(IntruderDebug, Verbose, TEXT("LoadGame - End"));
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
		UE_LOG(IntruderDebug, Error, TEXT("Player character null. Spawn WILL fail."));
	}

	AGameplayGM * GameplayGM = Cast<AGameplayGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameplayGM) {
		return;
	}

	GameplayGM->RespawnPlayer(this, PlayerSettings.MyPlayerCharacter);

	SetupChatWindow();

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

void AGameplayPC::SetupChatWindow_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SetupChatWindow_Implementation - Begin"));

	if (GameplayChatClass != nullptr) { // check if our widget class exists, else we'll crash
		if (GameplayChatWB == nullptr) { // init the widget
			GameplayChatWB = CreateWidget<UGameplayChat>(GetWorld(), GameplayChatClass);
		}

		GameplayChatWB->AddToViewport();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("SetupChatWindow_Implementation - End"));
}

void AGameplayPC::UpdateChat_Implementation(const FText &  SenderName, const FText &  SenderText)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateChat_Implementation - Begin"));

	if (GameplayChatWB->GetChatWindow()) {
		GameplayChatWB->GetChatWindow()->UpdateChatWindow(SenderName, SenderText);
	}
	else {
		UE_LOG(IntruderDebug, Warning, TEXT("Gameplay Chat Window not found. Messages won't appear on screen."));
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateChat_Implementation - End"));
}