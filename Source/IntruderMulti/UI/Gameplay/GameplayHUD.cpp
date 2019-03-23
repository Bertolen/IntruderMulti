// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayHUD.h"

void UGameplayHUD::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance) {
		DisplayedRemainingTime = FText::FromString(FString::Printf(TEXT("%d:00"), GameInstance->GameTime));
	}
}

void UGameplayHUD::UpdateRemainingTime(float ServerPlayTime)
{
	float TimeLeft = 0.0f;

	if (GameInstance) {
		TimeLeft = GameInstance->GameTime * 60 - ServerPlayTime;
	}

	int RemainingMinutes = TimeLeft / 60;
	int RemainingSeconds = TimeLeft - (RemainingMinutes * 60);

	DisplayedRemainingTime = FText::FromString(FString::Printf(TEXT("%d:%d"), RemainingMinutes, RemainingSeconds));
}