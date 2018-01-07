// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameWindow.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UEndGameWindow : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable)
		void Leave();

	////// Setters
	FORCEINLINE void SetWinText(const FString & NewText) { WinText = NewText; }
	
protected:
	UPROPERTY(BlueprintReadOnly)
		FString WinText;
	
};
