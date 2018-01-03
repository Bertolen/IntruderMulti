// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IntruderMulti/SaveGame/PlayerSaveGame.h"
#include "ConnectedPlayer.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UConnectedPlayer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerInfo(FPlayerInfo PlayerInfo);

protected:
	UPROPERTY(BlueprintReadOnly)
	FText NameOfPlayer;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D * PlayerAvatar;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D * ImageOfSelectedCharacter;

	UPROPERTY(BlueprintReadOnly)
	FText StatusOfPlayer;
};
