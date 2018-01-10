// Fill out your copyright notice in the Description page of Project Settings.

#include "EndGameWindow.h"
#include "Kismet/GameplayStatics.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"

void UEndGameWindow::Leave()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Leave - Begin"));

	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()->GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstance->DestroySessionCaller(GetOwningPlayer());

	UGameplayStatics::OpenLevel(GetOwningPlayer()->GetWorld(), "MainMenu");

	UE_LOG(IntruderDebug, Verbose, TEXT("Leave - End"));
}
