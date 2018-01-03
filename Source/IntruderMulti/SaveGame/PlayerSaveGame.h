// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/Texture2D.h"
#include "GameFramework/Character.h"
#include "PlayerSaveGame.generated.h"

UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	/** Default value, the player is not ready to launch the game yet */
	NotReady,
	/** The player is ready for the match to start */
	Ready,
	/** The player is hosting the lobby. He'll be the one to launch the game */
	Host
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	FPlayerInfo() : MyPlayerStatus(EPlayerStatus::NotReady) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
	FString MyPlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
	UTexture2D* MyPlayerImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
	TSubclassOf<ACharacter> MyPlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
	UTexture2D* MyPlayerCharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
	//FString MyPlayerStatus;
	EPlayerStatus MyPlayerStatus;
};

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vars)
	FPlayerInfo S_PlayerInfo;	
};
