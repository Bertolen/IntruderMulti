// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGM.h"
#include "IntruderMulti/PlayerController/GameplayPC.h"
#include "IntruderMulti/PlayerController/LobbyPC.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AGameplayGM::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameplayGM, AllPlayerControllers);
}

void AGameplayGM::SwapPlayerControllers(APlayerController * OldPC, APlayerController * NewPC)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("SwapPlayerControllers - Begin"));

	Super::SwapPlayerControllers(OldPC, NewPC);

	// keeps the list of player controllers to date
	AllPlayerControllers.Add(NewPC);

	//transmits important data from the old controller to the new controller
	AGameplayPC* GameplayPC = Cast<AGameplayPC>(NewPC);
	if (!GameplayPC) {
		return;
	}

	ALobbyPC* LobbyPC = Cast<ALobbyPC>(OldPC);
	if (!LobbyPC) {
		return;
	}

	GameplayPC->InitFromLobbyPC(LobbyPC);

	UE_LOG(IntruderDebug, Verbose, TEXT("SwapPlayerControllers - End"));
}

void AGameplayGM::Logout(AController* Exiting)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Logout - Begin"));

	Super::Logout(Exiting);

	AGameplayPC * GameplayPC = Cast<AGameplayPC>(Exiting);
	if (!GameplayPC) {
		return;
	}

	UGameInfoInstance * GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) {
		return;
	}

	GameInstance->DestroySessionCaller(GameplayPC);

	UE_LOG(IntruderDebug, Verbose, TEXT("Logout - End"));
}

bool AGameplayGM::RespawnPlayer_Validate(APlayerController* PlayerController, TSubclassOf<ACharacter> CharacterClass)
{
	return true;
}

void AGameplayGM::RespawnPlayer_Implementation(APlayerController* PlayerController, TSubclassOf<ACharacter> CharacterClass)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("RespawnPlayer - Begin"));

	// Destroy the previous pawn, if there's one
	if (PlayerController->GetPawn()) {
		PlayerController->GetPawn()->Destroy();
	}

	// Find a random point to spawn the character
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);
	int RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPoints.Num() - 1);
	FTransform SpawnTransform = SpawnPoints[RandomIndex]->GetActorTransform();

	// Spawn the character
	ACharacter * NewCharacter;
	if (CharacterClass) {
		NewCharacter = GetWorld()->SpawnActor<ACharacter>(CharacterClass, SpawnTransform);
	}
	else {
		NewCharacter = GetWorld()->SpawnActor<ACharacter>(DefaultPawnClass, SpawnTransform);
	}

	// Assing the new pawn to the player controller
	PlayerController->Possess(NewCharacter);

	UE_LOG(IntruderDebug, Verbose, TEXT("RespawnPlayer - End"));
}

void AGameplayGM::ThievesWin()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ThievesWin - Begin"));

	DisplayWinText("The relic has been stolen!");

	UE_LOG(IntruderDebug, Verbose, TEXT("ThievesWin - End"));
}

void AGameplayGM::GuardsWin()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("GuardsWin - Begin"));

	DisplayWinText("All thieves have been captured!");

	UE_LOG(IntruderDebug, Verbose, TEXT("GuardsWin - End"));
}

bool AGameplayGM::DisplayWinText_Validate(const FString & WinText)
{
	return true;
}

void AGameplayGM::DisplayWinText_Implementation(const FString & WinText)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("DisplayWinText_Implementation - Begin"));

	for (int i = 0; i < AllPlayerControllers.Num(); i++)
	{
		AGameplayPC* GameplayPC = Cast<AGameplayPC>(AllPlayerControllers[i]);
		if (!GameplayPC) {
			UE_LOG(IntruderDebug, Error, TEXT("Failed to cast player controller. Won't leave game properly."));
			return;
		}

		GameplayPC->DisplayEndGameWidget(WinText);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("DisplayWinText_Implementation - End"));
}