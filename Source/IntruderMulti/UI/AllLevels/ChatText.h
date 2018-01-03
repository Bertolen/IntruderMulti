// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "Blueprint/UserWidget.h"
#include "ChatText.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UChatText : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE FText GetText() const { return ActualText; }

	FORCEINLINE void SetText(const FText & NewText) { ActualText = NewText; }

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText ActualText;
};
