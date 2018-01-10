// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInfoInstance.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void UGameInfoInstance::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGameInfoInstance, MaxPlayers);
	DOREPLIFETIME(UGameInfoInstance, ServerName);
}

UGameInfoInstance::UGameInfoInstance(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{
	/** Bind function for CREATING a Session */
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UGameInfoInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UGameInfoInstance::OnStartOnlineGameComplete);

	/** Bind function for FINDING a Session */
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UGameInfoInstance::OnFindSessionsComplete);

	/** Bind function for JOINING a Session */
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UGameInfoInstance::OnJoinSessionComplete);

	/** Bind function for DESTROYING a Session */
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UGameInfoInstance::OnDestroySessionComplete);

	// Default values
	PlayerSettingsSave = "PlayerSettingsSave";
	CreatedSaveFile = false;
}

void UGameInfoInstance::Init()
{
	Super::Init();

	/** Errors handling **/
	GetEngine()->OnNetworkFailure().AddUObject(this, &UGameInfoInstance::OnNetworkError);
	GetEngine()->OnTravelFailure().AddUObject(this, &UGameInfoInstance::OnTravelError);
}

void UGameInfoInstance::ShowMainMenu()
{
	if (MainMenuClass != nullptr) { // check if our widget class exists, else we'll crash
		if (MainMenuWB == nullptr) { // init the widget
			MainMenuWB = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		}

		MainMenuWB->AddToViewport();
	}
}

void UGameInfoInstance::ShowHostMenu()
{
	if (HostMenuClass != nullptr) { // check if our widget class exists, else we'll crash
		if (HostMenuWB == nullptr) { // init the widget
			HostMenuWB = CreateWidget<UHostMenu>(GetWorld(), HostMenuClass);
		}

		HostMenuWB->AddToViewport();
	}
}

void UGameInfoInstance::ShowServerMenu()
{
	if (ServerMenuClass != nullptr) { // check if our widget class exists, else we'll crash
		if (ServerMenuWB == nullptr) { // init the widget
			ServerMenuWB = CreateWidget<UServerMenu>(GetWorld(), ServerMenuClass);
		}

		ServerMenuWB->AddToViewport();
	}
}

void UGameInfoInstance::ShowOptionsMenu()
{
	if (OptionsMenuClass != nullptr) { // check if our widget class exists, else we'll crash
		if (OptionsMenuWB == nullptr) { // init the widget
			OptionsMenuWB = CreateWidget<UOptionsMenu>(GetWorld(), OptionsMenuClass);
		}

		OptionsMenuWB->AddToViewport();
	}
}

void UGameInfoInstance::LaunchLobby(int NumberOfPlayers, bool EnableLan, FText ServerText)
{
	MaxPlayers = NumberOfPlayers;
	ServerName = ServerText;

	ShowLoadingScreen();
	HostSession(GetFirstGamePlayer()->GetPreferredUniqueNetId(), GameSessionName, true, true, NumberOfPlayers);
}

void UGameInfoInstance::ShowLoadingScreen()
{
	if (LoadingScreenClass != nullptr) { // check if our widget class exists, else we'll crash
		if (LoadingScreenWB == nullptr) { // init the widget
			LoadingScreenWB = CreateWidget<ULoadingScreen>(GetWorld(), LoadingScreenClass);
		}

		LoadingScreenWB->AddToViewport();
	}
}

void UGameInfoInstance::JoinServer(FOnlineSessionSearchResult SessionToJoin)
{
	ShowLoadingScreen();

	JoinSession(GetFirstGamePlayer()->GetPreferredUniqueNetId(), GameSessionName, SessionToJoin);
}

void UGameInfoInstance::OnNetworkError(UWorld * World, UNetDriver * NetDriver, ENetworkFailure::Type FailureType, const FString & NewErrorString)
{
	DisplayErrorMessage(NetErrorToString(FailureType));
}

void UGameInfoInstance::OnTravelError(UWorld * World, ETravelFailure::Type FailureType, const FString & NewErrorString)
{
	DisplayErrorMessage(TravelErrorToString(FailureType));
}

void UGameInfoInstance::DestroySessionCaller(APlayerController* PlayerController)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UGameInfoInstance::SaveGameCheck()
{
	if (UGameplayStatics::DoesSaveGameExist(PlayerSettingsSave, 0)) {
		ShowMainMenu();
		CreatedSaveFile = true;
	}
	else {
		ShowOptionsMenu();
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(nullptr);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
}

FString UGameInfoInstance::NetErrorToString(ENetworkFailure::Type FailureType)
{
	FString ErrorString;

	switch (FailureType)
	{
	case ENetworkFailure::ConnectionTimeout:
		ErrorString = "Error : Connection Timed Out";
		break;
	default:
		ErrorString = "Error : Lost Connection To Host";
		break;
	}
	return ErrorString;
}

FString UGameInfoInstance::TravelErrorToString(ETravelFailure::Type FailureType)
{
	FString ErrorString = "Error : Failed to travel. ";
	
	switch (FailureType)
	{
	case ETravelFailure::NoLevel:
		ErrorString.Append("No Level.");
		break;
	case ETravelFailure::LoadMapFailure:
		ErrorString.Append("Load Map Failure.");
		break;
	case ETravelFailure::InvalidURL:
		ErrorString.Append("Invalid URL.");
		break;
	case ETravelFailure::PackageMissing:
		ErrorString.Append("Package Missing.");
		break;
	case ETravelFailure::PackageVersion:
		ErrorString.Append("Package Version.");
		break;
	case ETravelFailure::NoDownload:
		ErrorString.Append("No Download.");
		break;
	case ETravelFailure::TravelFailure:
		ErrorString.Append("Travel Failure.");
		break;
	case ETravelFailure::CheatCommands:
		ErrorString.Append("Cheat Commands.");
		break;
	case ETravelFailure::PendingNetGameCreateFailure:
		ErrorString.Append("Pending Net Game Create Failure.");
		break;
	case ETravelFailure::CloudSaveFailure:
		ErrorString.Append("Cloud Save Failure.");
		break;
	case ETravelFailure::ServerTravelFailure:
		ErrorString.Append("Server Travel Failure.");
		break;
	case ETravelFailure::ClientTravelFailure:
		ErrorString.Append("Client Travel Failure.");
		break;
	default:
		ErrorString.Append("Unknown Error.");
		break;
	}

	return ErrorString;
}

void UGameInfoInstance::DisplayErrorMessage(FString ErrorMessage)
{
	if (ErrorDialogClass != nullptr) { // check if our widget class exists, else we'll crash

		if (ErrorDialogWB == nullptr) { // If the widget had never been made, we make one
			ErrorDialogWB = CreateWidget<UErrorDialog>(GetWorld(), ErrorDialogClass);
		}
		else { // if the widget already exists 
			if (ErrorDialogWB->IsInViewport()) { // If it's already visible we delete it before making a new one
				ErrorDialogWB->RemoveFromParent();
				ErrorDialogWB = CreateWidget<UErrorDialog>(GetWorld(), ErrorDialogClass);
			}
		}

		ErrorDialogWB->SetMessage(ErrorMessage);
		ErrorDialogWB->AddToViewport();
	}
}

bool UGameInfoInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	// Get the Online Subsystem to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
			Fill in all the Session Settings that we want to use.

			There are more with SessionSettings.Set(...);
			For example the Map or the GameMode/Type.
			*/
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, FString("Lobby"), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void UGameInfoInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
		}

	}
}

void UGameInfoInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Lobby", true, "listen");
	}
}

void UGameInfoInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence, int MaxResults)
{
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
			Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = MaxResults;
			SessionSearch->PingBucketSize = 50;

			// We only want to set this Query Setting if "bIsPresence" is true
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

void UGameInfoInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			// If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
			if (SessionSearch->SearchResults.Num() > 0)
			{
				// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
				// This can be customized later on with your own classes to add more information that can be set and displayed
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					// This is something you can't do in Blueprint for example!
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				}
			}
		}
	}
}

bool UGameInfoInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void UGameInfoInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));
	
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			APlayerController * const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				// Finally call the ClientTravel. If you want, you could print the TravelURL to see
				// how it really looks like
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Travelling to %s"), *TravelURL));
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UGameInfoInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			// If it was successful, we just load another level (could be a MainMenu!)
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "MainMenu", true);
			}
		}
	}
}