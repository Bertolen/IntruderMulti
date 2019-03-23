// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGS.h"

AGameplayGS::AGameplayGS(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.Constructor - Begin"));

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.Constructor - End"));
}

void AGameplayGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameplayGS, BeginPlayTime);
}

// Called when the game starts or when spawned
void AGameplayGS::BeginPlay()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.BeginPlay - Begin"));

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("AGameplayGS.BeginPlay")));

	Super::BeginPlay();

	BeginPlayTime = ReplicatedWorldTimeSeconds;

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.BeginPlay - End"));
}

void AGameplayGS::UpdateServerTimeSeconds()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.UpdateServerTimeSeconds - Begin"));
	
	Super::UpdateServerTimeSeconds();

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.UpdateServerTimeSeconds - End"));
}