// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IntruderMulti/SaveGame/PlayerSaveGame.h"
#include "PlayerButton.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UPlayerButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerInfo(FPlayerInfo NewPlayerInfo, int NewPlayerID);

protected:
	UFUNCTION(BlueprintCallable)
	void KickPlayer();

protected:
	UPROPERTY(Replicated)
	FPlayerInfo PlayerInfo;

	UPROPERTY()
	FText NameOfPlayer;

	UPROPERTY()
	UTexture2D * AvatarOfPlayer;

	UPROPERTY(Replicated)
	int PlayerID;
};
