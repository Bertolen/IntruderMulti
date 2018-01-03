// Fill out your copyright notice in the Description page of Project Settings.

#include "ConnectedPlayer.h"

void UConnectedPlayer::SetPlayerInfo(FPlayerInfo PlayerInfo)
{
	NameOfPlayer = FText::FromString(PlayerInfo.MyPlayerName);
	PlayerAvatar = PlayerInfo.MyPlayerImage;
	ImageOfSelectedCharacter = PlayerInfo.MyPlayerCharacterImage;

	switch (PlayerInfo.MyPlayerStatus)
	{
	case EPlayerStatus::Ready:
		StatusOfPlayer = FText::FromString("Ready");
		break;

	case EPlayerStatus::NotReady:
		StatusOfPlayer = FText::FromString("Not Ready");
		break;

	case EPlayerStatus::Host:
		StatusOfPlayer = FText::FromString("Host");
		break;

	default:
		StatusOfPlayer = FText::FromString("Unknown");
		break;
	}
}