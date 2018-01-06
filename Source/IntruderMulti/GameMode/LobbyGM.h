// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "GameFramework/GameModeBase.h"
#include "IntruderMulti/SaveGame/PlayerSaveGame.h"
#include "LobbyGM.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API ALobbyGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGM(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void PostLogin(APlayerController * NewPlayer) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void RespawnPlayer(APlayerController* PlayerController);

	UFUNCTION(Server, Reliable, WithValidation)
	void EveryoneUpdate();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerUpdateGameSettings(int MapID, int TimeID);

	UFUNCTION(BlueprintCallable)
	void LaunchTheGame();

	UFUNCTION(Server,Reliable, WithValidation, BlueprintCallable)
	void YouHaveBeenKicked(int PlayerID);

	virtual void Logout(AController* Exiting) override;

	void AddToKickList();

	UFUNCTION(Server, Reliable, WithValidation)
	void SwapCharacters(APlayerController* PlayerController, TSubclassOf<ACharacter> CharacterClass, bool ChangedStatus);

public:

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<ACharacter>> Characters;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<APlayerController*> AllPlayerControllers;

protected:

	UPROPERTY()
	TArray<AActor *> SpawnLocations;

	UPROPERTY()
	FText ServerName;

	UPROPERTY()
	int MaxPlayers;

	UPROPERTY(Replicated)
	int CurrentPlayers;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FPlayerInfo> ConnectedPlayers;
};
