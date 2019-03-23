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
	AGameplayGM(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void SwapPlayerControllers(APlayerController * OldPC, APlayerController * NewPC) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void RespawnPlayer(APlayerController* PlayerController, TSubclassOf<ACharacter> Character);

	void OneLessThief();

	void ThievesWin();

	void GuardsWin(FString DisplayText = "All thieves have been captured!");

	UFUNCTION(Server, Reliable, WithValidation)
	void DisplayWinText(const FString & WinText);

	////// Getters
	FORCEINLINE TArray<APlayerController*> GetAllPlayerControllers() { return AllPlayerControllers; }

	// Amount of seconds since the game started
	float GetPlayTime();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated)
		TArray<APlayerController*> AllPlayerControllers;

	UPROPERTY(Replicated)
		float BeginPlayTime;

	UPROPERTY()
		TArray<AActor*> SpawnPoints;

	UPROPERTY()
		int NbRemainingThieves;
};
