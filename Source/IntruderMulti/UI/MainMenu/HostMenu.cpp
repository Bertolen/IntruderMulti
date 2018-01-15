// Fill out your copyright notice in the Description page of Project Settings.

#include "HostMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"


UHostMenu::UHostMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NumberOfPlayers = 2;
	MaxNumberOfPlayers = 8;
	EnableLAN = false;
	PlayModeText = FText::FromString("INTERNET");
}

void UHostMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()->GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstanceRef = GameInstance;
}

void UHostMenu::UpdateServerName(FText NewServerName)
{
	ServerNameText = NewServerName;

	AcceptIsEnabled = !UKismetTextLibrary::TextIsEmpty(ServerNameText);
}

void UHostMenu::ShowMainMenu()
{
	RemoveFromParent();

	GameInstanceRef->ShowMainMenu();
}

void UHostMenu::LaunchLobby()
{
	RemoveFromParent();

	GameInstanceRef->LaunchLobby(NumberOfPlayers, EnableLAN, ServerNameText);
}

void UHostMenu::IncreasePlayers()
{
	NumberOfPlayers = UKismetMathLibrary::Clamp(NumberOfPlayers + 1, 2, MaxNumberOfPlayers);
}

void UHostMenu::DecreasePlayers()
{
	NumberOfPlayers = UKismetMathLibrary::Clamp(NumberOfPlayers - 1, 2, MaxNumberOfPlayers);
}

void UHostMenu::SetPlayModeLAN()
{
	if (!EnableLAN) {
		EnableLAN = true;
		PlayModeText = FText::FromString("LAN");
	}
}

void UHostMenu::SetPlayModeInternet()
{
	if (EnableLAN) {
		EnableLAN = false;
		PlayModeText = FText::FromString("INTERNET");
	}
}