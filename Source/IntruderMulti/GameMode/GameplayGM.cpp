// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGM.h"
#include "IntruderMulti/PlayerController/GameplayPC.h"
#include "IntruderMulti/PlayerController/LobbyPC.h"
#include "IntruderMulti/GameInstance/GameInfoInstance.h"
#include "IntruderMulti/Characters/FP_Characters/Guard.h"
#include "IntruderMulti/Characters/TP_Characters/Thief.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AGameplayGM::AGameplayGM(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AGameplayGM::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameplayGM, AllPlayerControllers);
}

void AGameplayGM::SwapPlayerControllers(APlayerController * OldPC, APlayerController * NewPC)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.SwapPlayerControllers - Begin"));

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

	AThief* isThief = Cast<AThief>(LobbyPC->PlayerSettings.MyPlayerCharacter.GetDefaultObject());
	if (isThief) {
		NbRemainingThieves++;
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.SwapPlayerControllers - End"));
}

void AGameplayGM::BeginPlay()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.BeginPlay - Begin"));

	Super::BeginPlay();

	// record the world time at this moment
	UWorld* World = GetWorld();
	if (World)
	{
		BeginPlayTime = World->GetTimeSeconds();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.BeginPlay - End"));
}

void AGameplayGM::Logout(AController* Exiting)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.Logout - Begin"));

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

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.Logout - End"));
}

void AGameplayGM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// retrieve the game instance
	UGameInfoInstance * GameInstance = Cast<UGameInfoInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) {
		return;
	}

	// Gives to all the clients the time spent playing
	for (int i = 0; i < AllPlayerControllers.Num(); i++)
	{
		AGameplayPC* GameplayPC = Cast<AGameplayPC>(AllPlayerControllers[i]);
		if (!GameplayPC) {
			UE_LOG(IntruderDebug, Error, TEXT("Failed to cast player controller. Won't leave game properly."));
			return;
		}

		GameplayPC->PassPlayTime(GetPlayTime());
	}

	// check if the time is up
	if (GameInstance->GameTime * 60 <= GetPlayTime()) {
		// if time is up then the guards have won
		GuardsWin();
	}
}

bool AGameplayGM::RespawnPlayer_Validate(APlayerController* PlayerController, TSubclassOf<ACharacter> CharacterClass)
{
	return true;
}

void AGameplayGM::RespawnPlayer_Implementation(APlayerController* PlayerController, TSubclassOf<ACharacter> CharacterClass)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.RespawnPlayer - Begin"));

	// Destroy the previous pawn, if there's one
	if (PlayerController->GetPawn()) {
		PlayerController->GetPawn()->Destroy();
	}

	// Find out the character's team
	bool bIsGuard = false;
	AGuard* Guard = Cast<AGuard>(CharacterClass.GetDefaultObject());
	if (Guard) {
		bIsGuard = true;
	}

	// Find all the spawn points
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);

	// Find the spawn points that don't match our team
	TArray<AActor*> SpawnsToRemove;
	for (int i = 0; i < SpawnPoints.Num(); i++)
	{
		APlayerStart * PlayerStart = Cast<APlayerStart>(SpawnPoints[i]);
		if (!PlayerStart) {
			UE_LOG(IntruderDebug, Error, TEXT("SpawnPoint is not a PlayerStart object."));
			continue;
		}

		// add the unmatching actors to the remove list
		if (bIsGuard && PlayerStart->PlayerStartTag != "Guard") {
			SpawnsToRemove.Add(SpawnPoints[i]);
		}
		else if (!bIsGuard && PlayerStart->PlayerStartTag != "Thief") {
			SpawnsToRemove.Add(SpawnPoints[i]);
		}
	}

	// Exclude the spawn points in the list
	for (int i = 0; i < SpawnsToRemove.Num(); i++)
	{
		SpawnPoints.Remove(SpawnsToRemove[i]);
	}

	// Find a random spawn point in the remaining list
	FTransform SpawnTransform;
	if (SpawnPoints.Num()) {
		int RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPoints.Num() - 1);
		SpawnTransform = SpawnPoints[RandomIndex]->GetActorTransform();
	}
	else {
		UE_LOG(IntruderDebug, Error, TEXT("No spawn points left. Spawning at origin by default."));
		SpawnTransform = FTransform::Identity;
	}

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

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.RespawnPlayer - End"));
}

void AGameplayGM::ThievesWin()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.ThievesWin - Begin"));

	DisplayWinText("The relic has been stolen!");

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.ThievesWin - End"));
}

void AGameplayGM::GuardsWin(FString DisplayText)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.GuardsWin - Begin"));

	DisplayWinText(DisplayText);

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.GuardsWin - End"));
}

void AGameplayGM::OneLessThief()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.OneLessThief - Begin"));

	NbRemainingThieves--;

	if (!NbRemainingThieves) {
		GuardsWin();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.OneLessThief - End"));
}

bool AGameplayGM::DisplayWinText_Validate(const FString & WinText)
{
	return true;
}

void AGameplayGM::DisplayWinText_Implementation(const FString & WinText)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.DisplayWinText_Implementation - Begin"));

	for (int i = 0; i < AllPlayerControllers.Num(); i++)
	{
		AGameplayPC* GameplayPC = Cast<AGameplayPC>(AllPlayerControllers[i]);
		if (!GameplayPC) {
			UE_LOG(IntruderDebug, Error, TEXT("Failed to cast player controller. Won't leave game properly."));
			return;
		}

		GameplayPC->DisplayEndGameWidget(WinText);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGM.DisplayWinText_Implementation - End"));
}

///////// GETTERS

float AGameplayGM::GetPlayTime()
{
	UWorld* World = GetWorld();
	if (World)
	{
		 return World->GetTimeSeconds() - BeginPlayTime;
	}

	return 0.0f;
}