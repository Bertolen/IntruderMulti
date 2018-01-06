// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelect.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UCharacterSelect : public UUserWidget
{
	GENERATED_BODY()

public:
	UCharacterSelect(const FObjectInitializer & ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION(BlueprintCallable)
		void SelectCharacterByID(int CharacterID);

	UFUNCTION(BlueprintCallable)
		void UpdateEnabledButtons();

protected:
	UPROPERTY()
		class UGameInfoInstance * GameInstanceRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TArray<class UButton *> CharacterButtons;

	UPROPERTY()
		FTimerHandle RetryUpdateEnabledButtons;
};
