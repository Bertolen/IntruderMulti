// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "IntruderMulti/UI/AllLevels/LoadingScreen.h"
#include "IntruderMulti/UI/MainMenu/MainMenu.h"
#include "IntruderMulti/UI/MainMenu/HostMenu.h"
#include "IntruderMulti/UI/MainMenu/OptionsMenu.h"
#include "IntruderMulti/UI/MainMenu/ErrorDialog.h"
#include "IntruderMulti/UI/MainMenu/ServerMenu.h"
#include "GameInfoInstance.generated.h"

class FOnlineSessionSearchResult;


/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UGameInfoInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGameInfoInstance(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

	//////////////////////////// variables
public:
	// Server Settings
	UPROPERTY(Replicated, BlueprintReadOnly)
	int MaxPlayers;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText ServerName;

	UPROPERTY(BlueprintReadWrite)
	bool CreatedSaveFile;

protected:

	UPROPERTY()
	FString PlayerSettingsSave;

	// widgets
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets|Widgets", meta = (BlueprintProtected = "true"))
	UMainMenu * MainMenuWB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets|Widgets", meta = (BlueprintProtected = "true"))
	UHostMenu * HostMenuWB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets|Widgets", meta = (BlueprintProtected = "true"))
	UOptionsMenu * OptionsMenuWB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets|Widgets", meta = (BlueprintProtected = "true"))
	ULoadingScreen * LoadingScreenWB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets|Widgets", meta = (BlueprintProtected = "true"))
	UErrorDialog* ErrorDialogWB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets|Widgets", meta = (BlueprintProtected = "true"))
	UServerMenu * ServerMenuWB;

	// widgets classes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets|Classes", meta = (BlueprintProtected = "true"))
	TSubclassOf<UMainMenu> MainMenuClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets|Classes", meta = (BlueprintProtected = "true"))
	TSubclassOf<UHostMenu> HostMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets|Classes", meta = (BlueprintProtected = "true"))
	TSubclassOf<UOptionsMenu> OptionsMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets|Classes", meta = (BlueprintProtected = "true"))
	TSubclassOf<ULoadingScreen> LoadingScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets|Classes", meta = (BlueprintProtected = "true"))
	TSubclassOf<UErrorDialog> ErrorDialogClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets|Classes", meta = (BlueprintProtected = "true"))
	TSubclassOf<UServerMenu> ServerMenuClass;

	//Delegates and Handles

	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	////////////////////////////// Functions
public:

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void ShowHostMenu();

	UFUNCTION(BlueprintCallable)
	void ShowServerMenu();

	UFUNCTION(BlueprintCallable)
	void ShowOptionsMenu();

	UFUNCTION(BlueprintCallable)
	void LaunchLobby(int NumberOfPlayers, bool EnableLan, FText ServerText);

	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen();

	void JoinServer(FOnlineSessionSearchResult SessionToJoin);

	UFUNCTION(BlueprintCallable)
	void OnNetworkError(UWorld * World, class UNetDriver * NetDriver, ENetworkFailure::Type FailureType, const FString & ErrorString);

	UFUNCTION(BlueprintCallable)
	void OnTravelError(UWorld * World, ETravelFailure::Type FailureType, const FString & ErrorString);

	UFUNCTION(BlueprintCallable)
	void DestroySessionCaller(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SaveGameCheck();

	UFUNCTION(BlueprintCallable)
	FString NetErrorToString(ENetworkFailure::Type FailureType);

	UFUNCTION(BlueprintCallable)
	FString TravelErrorToString(ETravelFailure::Type FailureType);

	// overrides 
	virtual void Init() override;

	////// Getters
	FORCEINLINE TArray<FOnlineSessionSearchResult> GetSessionsResult() const { return SessionSearch->SearchResults; }

private:
	UFUNCTION()
	void DisplayErrorMessage(FString ErrorMessage);

public:

	/**
	*	Function to host a game!
	*
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	*/
	bool HostSession(FUniqueNetIdRepl UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/**
	*	Function fired when a session create request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Find an online session
	*
	*	@param UserId user that initiated the request
	*	@param bIsLAN are we searching LAN matches
	*	@param bIsPresence are we searching presence sessions
	*	@param MaxResults Number of maximum sessions to find
	*/
	void FindSessions(FUniqueNetIdRepl UserId, bool bIsLAN, bool bIsPresence, int MaxResults);

	/**
	*	Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

	/**
	*	Joins a session via a search result
	*
	*	@param SessionName name of session
	*	@param SearchResult Session to join
	*
	*	@return bool true if successful, false otherwise
	*/
	bool JoinSession(FUniqueNetIdRepl UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	/**
	*	Delegate fired when a session join request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/**
	*	Delegate fired when a destroying an online session has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};
