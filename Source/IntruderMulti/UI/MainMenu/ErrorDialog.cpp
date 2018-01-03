// Fill out your copyright notice in the Description page of Project Settings.

#include "ErrorDialog.h"
#include "Kismet/GameplayStatics.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"


void UErrorDialog::CloseTheDialog()
{
	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()->GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstance->DestroySessionCaller(GetOwningPlayer());
	GameInstance->ShowMainMenu();
	RemoveFromParent();
}

void UErrorDialog::SetMessage(FString NewMessage)
{
	Message = NewMessage;
}