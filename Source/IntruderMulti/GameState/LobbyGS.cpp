// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGS.h"

ALobbyGS::ALobbyGS(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	MapID = 0;
	TimeID = 0;
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, MapID);
	DOREPLIFETIME(ALobbyGS, TimeID);
}

void ALobbyGS::SetMapID(const int NewMapId)
{
	if (0 <= NewMapId && NewMapId <= FMath::Min(AllMapNames.Num() - 1, AllMapImages.Num() - 1)) {
		MapID = NewMapId;
	}
}

void ALobbyGS::SetTimeID(const int NewTimeId)
{
	if (0 <= NewTimeId && NewTimeId <= AllTimes.Num() - 1) {
		TimeID = NewTimeId;
	}
}