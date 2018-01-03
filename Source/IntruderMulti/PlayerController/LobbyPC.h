// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "GameFramework/PlayerController.h"
#include "IntruderMulti/SaveGame/PlayerSaveGame.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPC(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());
	
public:

	////// Local functions

	virtual void SetupInputComponent() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void AssignPlayer(TSubclassOf<ACharacter> Character, UTexture2D * CharacterImage);

	void SaveGameCheck();

	void LoadGame();

	void SaveGame();

	UFUNCTION()
	void ToggleDisplay();

	////// Client functions

	UFUNCTION(Client, Reliable)
	void InitialSetup(const FText & ServerName);

	UFUNCTION(Client, Reliable)
	void SetupLobbyMenu(const FText & ServerName);

	UFUNCTION(Client, Reliable)
	void AddPlayerInfo(const TArray<FPlayerInfo> & ConnectedPlayersInfo);

	UFUNCTION(Client, Reliable)
	void UpdateLobbySettings();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ShowLoadingScreen();

	UFUNCTION(Client, Reliable)
	void UpdateNumberOfPlayers(int CurrentPlayers, int MaxPlayers);

	UFUNCTION(Client, Reliable)
	void Kicked();

	UFUNCTION(Client, Reliable)
	void UpdateTakenCharacters(const TArray<bool> & NewTakenCharacters);

	UFUNCTION(Client, Reliable)
	void UpdateChat(const FText & Sender, const FText & Text);

	/////// Server functions

	UFUNCTION(Server, Reliable, WithValidation)
	void AssignSelectedCharacter(int CharacterID);

	UFUNCTION(Server, Reliable, WithValidation)
	void GetChatMessage(const FText & TextToSend);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void CallUpdate(FPlayerInfo PlayerInfo, bool ChangedStatus);

	UFUNCTION(Server, Reliable, WithValidation)
	void CharacterCheck();

public:

	UPROPERTY()
	FString PlayerSettingsSave;

	UPROPERTY()
	UPlayerSaveGame * PlayerSaveGameRef;

	UPROPERTY()
	class ULobbyMenu* LobbyMenuWB;

	// widgets classes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets|Classes", meta = (BlueprintProtected = "true"))
	TSubclassOf<class ULobbyMenu> LobbyMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets|Classes", meta = (BlueprintProtected = "true"))
	TSubclassOf<class ULoadingScreen> LoadingScreenClass;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FPlayerInfo PlayerSettings;

	UPROPERTY(Replicated)
	TArray<FPlayerInfo> AllConnectedPlayers;

	UPROPERTY(Replicated)
	int SelectedCharacter;

	UPROPERTY(Replicated)
	TArray<bool> TakenCharacters;

	UPROPERTY(Replicated)
	int PreviousSelection;

	UPROPERTY(Replicated)
	UTexture2D* CharacterImage;

	UPROPERTY(Replicated)
	FText SenderText;

	UPROPERTY(Replicated)
	FText SenderName;

private:
	FTimerHandle InitLobbySettingsTimerHandle;
};
