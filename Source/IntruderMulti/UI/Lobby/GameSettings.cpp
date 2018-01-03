// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "IntruderMulti/GameMode/LobbyGM.h"

UGameSettings::UGameSettings(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UGameSettings::NativeConstruct()
{
	Super::NativeConstruct();

	MapID = 0;
	TimeID = 0;

	GameState = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetOwningPlayer()->GetWorld()));
	if (GameState) {
		DisplayedMap = GameState->GetMapImage();
		DisplayedMapName = GameState->GetMapName();
		DisplayedMapTime = GameState->GetTime();
	}
}

void UGameSettings::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGameSettings, DisplayedMap);
	DOREPLIFETIME(UGameSettings, DisplayedMapTime);
	DOREPLIFETIME(UGameSettings, DisplayedMapName);
	DOREPLIFETIME(UGameSettings, MapID);
	DOREPLIFETIME(UGameSettings, TimeID);
}

void UGameSettings::SelectNextMap()
{
	int NumberOfMaps = UKismetMathLibrary::Min(GetGameState()->GetAllMaps().Num(), GetGameState()->GetAllMapNames().Num());
	MapID = UKismetMathLibrary::Clamp(MapID + 1, 0, NumberOfMaps - 1);

	SelectMapByMapID(MapID);
}

void UGameSettings::SelectPreviousMap()
{
	int NumberOfMaps = UKismetMathLibrary::Min(GetGameState()->GetAllMaps().Num(), GetGameState()->GetAllMapNames().Num());
	MapID = UKismetMathLibrary::Clamp(MapID - 1, 0, NumberOfMaps - 1);

	SelectMapByMapID(MapID);
}

void UGameSettings::SelectMapByMapID(int NewMapID)
{
	MapID = NewMapID;

	GetGameState()->SetMapID(NewMapID);

	DisplayedMap = GetGameState()->GetMapImage();
	DisplayedMapName = GetGameState()->GetMapName();
}

void UGameSettings::SelectNextTime()
{
	TimeID = UKismetMathLibrary::Clamp(TimeID + 1, 0, GetGameState()->GetAllTimes().Num() - 1);

	SelectTimeByTimeID(TimeID);
}

void UGameSettings::SelectPreviousTime()
{
	TimeID = UKismetMathLibrary::Clamp(TimeID - 1, 0, GetGameState()->GetAllTimes().Num() - 1);

	SelectTimeByTimeID(TimeID);
}

void UGameSettings::SelectTimeByTimeID(int NewTimeID)
{
	TimeID = NewTimeID;

	GetGameState()->SetTimeID(NewTimeID);

	DisplayedMapTime = GetGameState()->GetTime();
}

void UGameSettings::UpdateGameSettings()
{
	ALobbyGM * LobbyGM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetOwningPlayer()->GetWorld()));
	if (!LobbyGM) {
		return;
	}
	LobbyGM->ServerUpdateGameSettings(MapID,TimeID);

	SetVisibility(ESlateVisibility::Hidden);
}