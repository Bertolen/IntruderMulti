// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "Blueprint/UserWidget.h"
#include "IntruderMulti/GameState/LobbyGS.h"
#include "GameSettings.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UGameSettings : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameSettings(const FObjectInitializer & ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void FillPlayersWindow();

	/////// Getters
	FORCEINLINE ALobbyGS * GetGameState() const { return GameState; }

protected:
	UFUNCTION(BlueprintCallable)
		void SelectNextMap();

	UFUNCTION(BlueprintCallable)
		void SelectPreviousMap();

	UFUNCTION(BlueprintCallable)
		void SelectNextTime();

	UFUNCTION(BlueprintCallable)
		void SelectPreviousTime();

	UFUNCTION(BlueprintCallable)
		void UpdateGameSettings();

	UFUNCTION(BlueprintImplementableEvent)
		void ShowSettingsWidget();

	UFUNCTION(BlueprintImplementableEvent)
		void ShowKickWidget();

	void SelectMapByMapID(int NewMapID);

	void SelectTimeByTimeID(int NewTimeID);

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D * DisplayedMap;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText DisplayedMapTime;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText DisplayedMapName;

	UPROPERTY(Replicated)
	int MapID;

	UPROPERTY(Replicated)
	int TimeID;

private:
	ALobbyGS* GameState;
};
