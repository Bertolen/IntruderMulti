// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/*
includes that might become usefull dow the road :
"Runtime/UMG/Public/UMG.h"
"Runtime/UMG/Public/UMGStyle.h"
"Runtime/UMG/Public/Slate/SObjectWidget.h"
"Runtime/UMG/Public/IUMGModule.h"
"Runtime/UMG/Public/Blueprint/UserWidget.h"
*/

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	virtual void ShowHostMenu();

	UFUNCTION(BlueprintCallable)
	virtual void ShowServerMenu();

	UFUNCTION(BlueprintCallable)
	virtual void ShowOptionsMenu();

	UFUNCTION(BlueprintCallable)
	virtual void QuitGame();
	
protected:
	UPROPERTY()
	class UGameInfoInstance * GameInstanceRef;
	
};
