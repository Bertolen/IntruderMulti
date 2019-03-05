// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"

UServerMenu::UServerMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SessionFound = false;
	PlayModeHeader = "Choose Play Method";
	TimerValue = 10;
	ButtonVisibility = ESlateVisibility::Visible;
	NumOfSessionsResult = 20;

	/** Bind function for FINDING a Session */
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UServerMenu::OnSearchComplete);
}

void UServerMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInfoInstance* GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()->GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstanceRef = GameInstance;

	PlayModeText = "INTERNET";
	EnableLAN = false;
}

void UServerMenu::SetSearchModeLAN()
{
	if (!EnableLAN) {
		EnableLAN = true;
		PlayModeText = "LAN";
	}
}

void UServerMenu::SetSearchModeInternet()
{
	if (EnableLAN) {
		EnableLAN = false;
		PlayModeText = "INTERNET";
	}
}

void UServerMenu::ShowMainMenu()
{
	RemoveFromParent();

	GameInstanceRef->ShowMainMenu();
}

void UServerMenu::StartSearch()
{
	ButtonVisibility = ESlateVisibility::Hidden;
	PlayModeHeader = "Searching...";
	FoundSessions.Empty();
	ChangeSwitcherActiveWidget(1);

	///// Getting the delegate to subscribe to it

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (!OnlineSub) {
		OnSearchComplete(false);
		return;
	}
	// Get the SessionInterface from our OnlineSubsystem
	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	FUniqueNetIdRepl UserId = GetOwningPlayer()->GetLocalPlayer()->GetPreferredUniqueNetId();

	if (!Sessions.IsValid() || !UserId.IsValid()) {
		OnSearchComplete(false);
		return;
	}

	// Set the Delegate to the Delegate Handle of the FindSession function
	OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	GameInstanceRef->FindSessions(UserId, EnableLAN, true, NumOfSessionsResult);
}

void UServerMenu::OnSearchComplete(bool bWasSuccessful)
{
	ButtonVisibility = ESlateVisibility::Visible;

	if (!bWasSuccessful) {
		OnSearchFailed();
		return;
	}

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (!OnlineSub) {
		OnSearchFailed();
		return;
	}
	
	// Get the SessionInterface from our OnlineSubsystem
	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	FUniqueNetIdRepl UserId = GetOwningPlayer()->GetLocalPlayer()->GetPreferredUniqueNetId();

	if (!Sessions.IsValid() || !UserId.IsValid()) {
		OnSearchFailed();
		return;
	}

	// Clear the Delegate handle, since we finished this call
	Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

	// Search for a available session
	FoundSessions = GameInstanceRef->GetSessionsResult();
	for (int i = 0; i < FoundSessions.Num(); i++)
	{
		if (FoundSessions[i].Session.NumOpenPublicConnections >= 0) { // If there's still room in the given session
			SessionFound = true;
			AvailableSession = FoundSessions[i];
		}
	}

	// If we found a session we display it, else we display an error message
	if (SessionFound) {
		DisplaySession(AvailableSession);
		ChangeSwitcherActiveWidget(2);
	}
	else {
		PlayModeHeader = "No Sessions Found. Please Try Again.";
		ChangeSwitcherActiveWidget(0);
	}
}

void UServerMenu::OnSearchFailed()
{
	PlayModeHeader = "Search Failed. Please Try Again.";
	ChangeSwitcherActiveWidget(0);
}

void UServerMenu::DisplaySession(FOnlineSessionSearchResult SessionToJoin)
{
	AvailableSession = SessionToJoin;
	PlayModeHeader = "Match Available. Join The Match?";
	
	GetOwningPlayer()->GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UServerMenu::SessionTimer, 1.0f, true);
}

void UServerMenu::JoinSession()
{
	RemoveFromParent();

	GameInstanceRef->JoinServer(AvailableSession);
}

void UServerMenu::SessionTimer()
{
	if (SessionFound) {
		TimerValue--;
		if (TimerValue == 0) {
			SessionFound = false;
			TimerValue = 10;
			ButtonVisibility = ESlateVisibility::Visible;
			PlayModeHeader = "Choose Play Method.";

			GetOwningPlayer()->GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
			ChangeSwitcherActiveWidget(0);
		}
	}
}