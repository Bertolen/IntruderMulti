// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()->GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstanceRef = GameInstance;
}

void UMainMenu::ShowHostMenu()
{
	RemoveFromParent();

	GameInstanceRef->ShowHostMenu();
}

void UMainMenu::ShowServerMenu() 
{
	RemoveFromParent();

	GameInstanceRef->ShowServerMenu();
}

void UMainMenu::ShowOptionsMenu()
{
	RemoveFromParent();

	GameInstanceRef->ShowOptionsMenu();
}

void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetOwningPlayer()->GetWorld(), GetOwningPlayer(), EQuitPreference::Quit);
}