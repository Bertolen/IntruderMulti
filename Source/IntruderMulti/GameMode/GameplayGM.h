// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayGM.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API AGameplayGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void SwapPlayerControllers(APlayerController * OldPC, APlayerController * NewPC) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void RespawnPlayer(APlayerController* PlayerController, TSubclassOf<ACharacter> Character);

	void ThievesWin();

	void GuardsWin();

	UFUNCTION(Server, Reliable, WithValidation)
	void DisplayWinText(const FString & WinText);

	////// Getters
	FORCEINLINE TArray<APlayerController*> GetAllPlayerControllers() { return AllPlayerControllers; }

private:
	UPROPERTY(Replicated)
		TArray<APlayerController*> AllPlayerControllers;

	UPROPERTY()
		TArray<AActor*> SpawnPoints;
};
