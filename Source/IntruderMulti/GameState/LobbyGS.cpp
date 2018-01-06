// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGS.h"
#include "IntruderMulti/GameMode/LobbyGM.h"

ALobbyGS::ALobbyGS(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	MapID = 0;
	TimeID = 0;
}

void ALobbyGS::BeginPlay()
{
	Super::BeginPlay();

	ALobbyGM * LobbyGM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!LobbyGM) {
		UE_LOG(IntruderDebug, Error, TEXT("Could not get game mode. Game state won't be properly initialized."));
		return;
	}

	for (int i = 0; i < LobbyGM->Characters.Num(); i++)
	{
		TakenCharacters.Add(false);
	}
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, MapID);
	DOREPLIFETIME(ALobbyGS, TimeID);
	DOREPLIFETIME(ALobbyGS, CanWeStart);
	DOREPLIFETIME(ALobbyGS, TakenCharacters);
}

////// Setters

bool ALobbyGS::SetMapID_Validate(const int NewMapId)
{
	if (0 <= NewMapId && NewMapId <= FMath::Min(AllMapNames.Num() - 1, AllMapImages.Num() - 1)) {
		return true;
	}

	return false;
}

void ALobbyGS::SetMapID_Implementation(const int NewMapId)
{
	MapID = NewMapId;
}

bool ALobbyGS::SetTimeID_Validate(const int NewTimeId)
{
	if (0 <= NewTimeId && NewTimeId <= AllTimes.Num() - 1) {
		return true;
	}

	return false;
}

void ALobbyGS::SetTimeID_Implementation(const int NewTimeId)
{
	TimeID = NewTimeId;
}

bool ALobbyGS::SetCanWeStart_Validate(const bool NewCanWeStart)
{
	return true;
}

void ALobbyGS::SetCanWeStart_Implementation(const bool NewCanWeStart)
{
	CanWeStart = NewCanWeStart;
}

bool ALobbyGS::SetTakenCharacterByIndex_Validate(const int CharacterID, const bool TakenCharacter)
{
	if (0 <= CharacterID && CharacterID <= TakenCharacters.Num() - 1) {
		return true;
	}

	return false;
}

void ALobbyGS::SetTakenCharacterByIndex_Implementation(const int CharacterID, const bool TakenCharacter)
{
	TakenCharacters[CharacterID] = TakenCharacter;
}