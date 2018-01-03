// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IntruderMulti/SaveGame/PlayerSaveGame.h"
#include "OptionsMenu.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UOptionsMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOptionsMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	void SaveGameCheck();

	UFUNCTION(BlueprintCallable)
	void EmptyNameCheck(FText NameText);

	void LoadGame();

	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void SaveOptions();

	UFUNCTION(BlueprintCallable)
	void CycleToNextAvatarImage();

	UFUNCTION(BlueprintCallable)
	void CycleToPreviousAvatarImage();
	
protected:
	UPROPERTY()
	class UGameInfoInstance * GameInstanceRef;

	UPROPERTY()
	UPlayerSaveGame * SaveGameRef;

	UPROPERTY(BlueprintReadOnly)
	bool AcceptIsEnabled;

	UPROPERTY(BlueprintReadOnly)
	ESlateVisibility WelcomeMessageVis;

	UPROPERTY(BlueprintReadOnly)
	FText EnteredPlayerName;

	UPROPERTY()
	FPlayerInfo PlayerInfo;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D * AvatarImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> AllAvatars;

	UPROPERTY()
	int AvatarCount;

	UPROPERTY()
	FString PlayerSettingsSave;
};
