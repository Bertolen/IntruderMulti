// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Classes/FindSessionsCallbackProxy.h"
#include "ServerMenu.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UServerMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UServerMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetSearchModeLAN();

	UFUNCTION(BlueprintCallable)
	void SetSearchModeInternet();

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void StartSearch();

	UFUNCTION()
	void OnSearchComplete(bool bWasSuccessful);

	void OnSearchFailed();

	void DisplaySession(FOnlineSessionSearchResult SessionToJoin);

	UFUNCTION(BlueprintCallable)
	void JoinSession();

	void SessionTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeSwitcherActiveWidget(int ActiveWidgetIndex);

protected:
	UPROPERTY()
	class UGameInfoInstance * GameInstanceRef;

	UPROPERTY(BlueprintReadOnly)
	FString PlayModeText;

	UPROPERTY()
	bool EnableLAN;

	UPROPERTY(BlueprintReadOnly)
	bool SessionFound;

	UPROPERTY(BlueprintReadOnly)
	FString PlayModeHeader;

	FOnlineSessionSearchResult AvailableSession;

	UPROPERTY(BlueprintReadOnly)
	int TimerValue;

	UPROPERTY(BlueprintReadOnly)
	ESlateVisibility ButtonVisibility;

	TArray<FOnlineSessionSearchResult> FoundSessions;

	UPROPERTY()
	int NumOfSessionsResult;

	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	FTimerHandle CountdownTimerHandle;
};
