// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ErrorDialog.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UErrorDialog : public UUserWidget
{
	GENERATED_BODY()
	
private:
	
	UFUNCTION(BlueprintCallable)
	void CloseTheDialog();

public:
	void SetMessage(FString NewMessage);

protected:
	UPROPERTY(BlueprintReadOnly)
	FString Message;

};
