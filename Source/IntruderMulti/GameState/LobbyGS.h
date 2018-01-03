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
	void SetMapID(const int NewMapId);

	void SetTimeID(const int NewTimeId);

	///////// Getters
	FORCEINLINE int GetMapID() const { return MapID; }

	FORCEINLINE int GetTimeID() const { return TimeID; }

	FORCEINLINE TArray<UTexture2D*> GetAllMaps() const { return AllMapImages; }

	FORCEINLINE TArray<FText> GetAllMapNames() const { return AllMapNames; }

	FORCEINLINE TArray<FText> GetAllTimes() const { return AllTimes; }

	FORCEINLINE UTexture2D* GetMapImage() const { return AllMapImages[MapID]; }

	FORCEINLINE FText GetMapName() const { return AllMapNames[MapID]; }

	FORCEINLINE FText GetTime() const { return AllTimes[TimeID]; }

	FORCEINLINE TArray<UTexture2D*> GetAllCharacterImages() const { return AllCharacterImages; }

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		int MapID;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		int TimeID;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		TArray<UTexture2D*> AllMapImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		TArray<FText> AllMapNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Maps")
		TArray<FText> AllTimes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "LobbyGS|Images|Characters")
		TArray<UTexture2D*> AllCharacterImages;
};
