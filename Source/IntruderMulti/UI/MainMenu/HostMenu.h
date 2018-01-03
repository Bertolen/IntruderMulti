// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostMenu.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UHostMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UHostMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateServerName(FText NewServerName);

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void LaunchLobby();

	UFUNCTION(BlueprintCallable)
	void IncreasePlayers();

	UFUNCTION(BlueprintCallable)
	void DecreasePlayers();

	UFUNCTION(BlueprintCallable)
	void SetPlayModeLAN();

	UFUNCTION(BlueprintCallable)
	void SetPlayModeInternet();

protected:
	UPROPERTY()
	class UGameInfoInstance * GameInstanceRef;

	UPROPERTY(BlueprintReadOnly)
	FText ServerNameText;

	UPROPERTY(BlueprintReadOnly)
	bool AcceptIsEnabled;

	UPROPERTY()
	bool EnableLAN;

	UPROPERTY(BlueprintReadOnly)
	int NumberOfPlayers;

	UPROPERTY()
	int MaxNumberOfPlayers;

	UPROPERTY(BlueprintReadOnly)
	FText PlayModeText;
};
