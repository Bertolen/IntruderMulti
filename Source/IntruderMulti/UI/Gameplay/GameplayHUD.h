// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"
#include "IntruderMulti/UI/AllLevels/ChatWindow.h"
#include "IntruderMulti/UI/Gameplay/GameplayMenu.h"
#include "GameplayHUD.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UGameplayHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

public:
	void ShowEndGameWindow(FString Wintext);

	void UpdateRemainingTime(float ServerPlayTime);

	void ShowMenuWindow();
	
	//// Getters 
	FORCEINLINE UChatWindow* GetChatWindow() { return ChatWindowWB; }	

	FORCEINLINE UGameplayMenu* GetMenuWindow() { return GameplayMenuWB; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText DisplayedRemainingTime;

	UGameInfoInstance * GameInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
		class UChatWindow* ChatWindowWB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
		class UEndGameWindow* EndGameWindowWB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
		class UGameplayMenu* GameplayMenuWB;
};
