// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayMenu.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UGameplayMenu : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:
	UFUNCTION(BlueprintCallable)
		void LeaveGame();
	
	UFUNCTION(BlueprintCallable)
		void Hide();
};
