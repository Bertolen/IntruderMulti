// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerButton.h"
#include "IntruderMulti/GameMode/LobbyGM.h"
#include "Kismet/GameplayStatics.h"


void UPlayerButton::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerButton, PlayerInfo);
	DOREPLIFETIME(UPlayerButton, PlayerID);
}

void UPlayerButton::SetPlayerInfo(FPlayerInfo NewPlayerInfo, int NewPlayerID)
{
	PlayerID = NewPlayerID;
	PlayerInfo = NewPlayerInfo;
	AvatarOfPlayer = PlayerInfo.MyPlayerImage;
	NameOfPlayer = FText::FromString(PlayerInfo.MyPlayerName);
}

void UPlayerButton::KickPlayer()
{
	ALobbyGM* LobbyGM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetOwningPlayer()->GetWorld()));
	if (!LobbyGM) {
		return;
	}

	LobbyGM->YouHaveBeenKicked(PlayerID);
	RemoveFromParent();
}