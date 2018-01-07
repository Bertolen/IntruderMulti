// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "GameFramework/PlayerController.h"
#include "IntruderMulti/SaveGame/PlayerSaveGame.h"
#include "GameplayPC.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API AGameplayPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGameplayPC(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void BeginPlay() override;

	UFUNCTION()
		void ToggleDisplay();

	UFUNCTION(Server, Reliable, WithValidation)
		void PassCharacterInfoToServer(FPlayerInfo PlayerSettingsInfo);

	UFUNCTION(Server, Reliable, WithValidation)
		void GetChatMessage(const FText & Text);

	UFUNCTION(Client, Reliable)
		void SetupChatWindow();

	UFUNCTION()
		void ShowMenuWindow();

	UFUNCTION(Client, Reliable)
		void UpdateChat(const FText & SenderName, const FText & SenderText);

	UFUNCTION(Server, Reliable, WithValidation)
		void InitFromLobbyPC(class ALobbyPC * LobbyPC);

protected:
	UPROPERTY()
		FString PlayerSettingsSave;
	
	UPROPERTY(Replicated)
		FPlayerInfo PlayerSettings;

	UPROPERTY()
		FText SenderText;

	UPROPERTY(Replicated)
		FText SenderName;

	UPROPERTY()
		class UGameplayChat* GameplayChatWB;

	UPROPERTY()
		class UGameplayMenu* GameplayMenuWB;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayPC|WidgetClasses", meta = (AlowPrivateAccess = "true"))
		TSubclassOf<class UGameplayChat> GameplayChatClass;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayPC|WidgetClasses", meta = (AlowPrivateAccess = "true"))
		TSubclassOf<class UGameplayMenu> GameplayMenuClass;
};