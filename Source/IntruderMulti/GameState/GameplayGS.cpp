// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGS.h"

AGameplayGS::AGameplayGS(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.Constructor - Begin"));

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.Constructor - End"));
}

void AGameplayGS::UpdateServerTimeSeconds()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.UpdateServerTimeSeconds - Begin"));
	
	Super::UpdateServerTimeSeconds();

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("ReplicatedWorldTimeSeconds %f"), this->ReplicatedWorldTimeSeconds));

	UE_LOG(IntruderDebug, Verbose, TEXT("AGameplayGS.UpdateServerTimeSeconds - End"));
}