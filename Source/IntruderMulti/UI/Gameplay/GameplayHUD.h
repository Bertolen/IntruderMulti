// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"
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
	void UpdateRemainingTime(float ServerPlayTime);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText DisplayedRemainingTime;

	UGameInfoInstance * GameInstance;
};
