// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "Blueprint/UserWidget.h"
#include "IntruderMulti/SaveGame/PlayerSaveGame.h"
#include "IntruderMulti/UI/AllLevels/ChatWindow.h"
#include "IntruderMulti/UI/Lobby/ConnectedPlayer.h"
#include "IntruderMulti/UI/Lobby/CharacterSelect.h"
#include "IntruderMulti/UI/Lobby/GameSettings.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void ClearPlayerList();

	UFUNCTION(BlueprintImplementableEvent)
		void AddPlayerToList(UConnectedPlayer* PlayerWidget);

	UFUNCTION(BlueprintImplementableEvent)
		void ToggleDisplay();

	UFUNCTION(Client, Reliable)
		void UpdatePlayerWindow(FPlayerInfo IncomingPlayerInfo);

	////// Getters

	FORCEINLINE UCharacterSelect * GetCharacterSelectWB() const { return CharacterSelectWB; }

	FORCEINLINE UGameSettings * GetGameSettingsWB() const { return GameSettingsWB; }

	FORCEINLINE UChatWindow * GetChatWindowWB() const { return ChatWindowWB; }

	FORCEINLINE EPlayerStatus GetReadyStatus() const { return ReadyStatus; }

	////// Setters
	void SetReadyStatus(const EPlayerStatus & NewStatus);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void LeaveLobby();

	UFUNCTION(BlueprintCallable)
	void ToggleStatus();

	UFUNCTION(BlueprintCallable)
	void LaunchTheGame();

	UFUNCTION(BlueprintCallable)
	bool EnabledReadyStartButton();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "LobbyMenu|MapInfo")
	FText MapName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "LobbyMenu|MapInfo")
	FText MapTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "LobbyMenu|MapInfo")
	UTexture2D * MapImage;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FText PlayersDisplay;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FText LobbyServerName;

protected:
	UPROPERTY()
	class ALobbyPC * LobbyPcRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	UCharacterSelect * CharacterSelectWB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	UGameSettings * GameSettingsWB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	UChatWindow * ChatWindowWB;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FText ReadyButtonText;

	UPROPERTY(BlueprintReadOnly, Replicated)
	EPlayerStatus ReadyStatus;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "LobbyMenu", meta = (BlueprintProtected = "true"))
	TSubclassOf<APawn> DefaultPawnClass;

	// widgets classes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LobbyMenu|WidgetClasses", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UConnectedPlayer> ConnectedPlayerClass;
};
