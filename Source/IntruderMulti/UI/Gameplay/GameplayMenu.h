// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IntruderMulti/UI/AllLevels/ChatWindow.h"
#include "GameplayMenu.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UGameplayMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowEndGameWindow(FString Wintext);

	UFUNCTION(BlueprintCallable)
		void Hide();

	void ShowMenu();

	////// Getters 
	FORCEINLINE UChatWindow* GetChatWindow() { return ChatWindowWB; }	
	
protected:
	UFUNCTION(BlueprintCallable)
		void LeaveGame();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
		class UChatWindow* ChatWindowWB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
		class UEndGameWindow* EndGameWindowWB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
		class UBorder* MenuWB;
};
