// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ALobbyGS(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

public:
	///////// Setters
	UFUNCTION(Server, Reliable, WithValidation)
	void SetMapID(const int NewMapId);

	UFUNCTION(Server, Reliable, WithValidation)
	void SetTimeID(const int NewTimeId);

	UFUNCTION(Server, Reliable, WithValidation)
	void SetCanWeStart(const bool NewCanWeStart);

	UFUNCTION(Server, Reliable, WithValidation)
	void SetTakenCharacterByIndex(const int CharacterID, const bool TakenCharacter);

	///////// Getters
	FORCEINLINE int GetMapID() const { return MapID; }

	FORCEINLINE int GetTimeID() const { return TimeID; }

	FORCEINLINE TArray<UTexture2D*> GetAllMaps() const { return AllMapImages; }

	FORCEINLINE TArray<FText> GetAllMapNames() const { return AllMapNames; }

	FORCEINLINE TArray<int> GetAllTimes() const { return AllTimes; }

	FORCEINLINE UTexture2D* GetMapImage() const { return AllMapImages[MapID]; }

	FORCEINLINE FText GetMapName() const { return AllMapNames[MapID]; }

	FORCEINLINE int GetTime() const { return AllTimes[TimeID]; }

	FORCEINLINE TArray<UTexture2D*> GetAllCharacterImages() const { return AllCharacterImages; }

	FORCEINLINE bool GetCanWeStart() const { return CanWeStart; }

	FORCEINLINE TArray<bool> GetTakenCharacters() const { return TakenCharacters; }

	FORCEINLINE bool GetTakenCharacterByIndex(const int CharacterID) const { return TakenCharacters[CharacterID]; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		int MapID;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		int TimeID;

	UPROPERTY(Replicated, BlueprintReadOnly)
		bool CanWeStart;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		TArray<UTexture2D*> AllMapImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		TArray<FText> AllMapNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		TArray<int> AllTimes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Images|Characters")
		TArray<UTexture2D*> AllCharacterImages;

	UPROPERTY(Replicated)
		TArray<bool> TakenCharacters;
};
