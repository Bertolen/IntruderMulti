// Fill out your copyright notice in the Description page of Project Settings.

#include "IntruderMovementComponent.h"

UIntruderMovementComponent::UIntruderMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// let the object replicate (for multicast functions)
	bReplicates = true;

	// init some values
	bIsRunning = false;
	bCanRun = false;
	WalkingSpeed = 600.0f;
	RunningSpeed = 800.0f;
	MaxWalkSpeedCrouched = WalkingSpeed / 2;
	MaxWalkSpeed = WalkingSpeed;
}

void UIntruderMovementComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UIntruderMovementComponent, bIsRunning);
}


///////////////////////// ACCESSORS ////////////////////////////

bool UIntruderMovementComponent::SetIsRunning_Validate(bool newValue) 
{
	return true;
}

void UIntruderMovementComponent::SetIsRunning_Implementation(bool newValue) 
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Set is running")));
	// if the character can't run, then we don't even try
	if (!bCanRun) {
		bIsRunning = false;
		return;
	}

	// if he can we update his speed values
	bIsRunning = newValue;

	if (bIsRunning) {
		bIsRunning = true;
		MaxWalkSpeed = FMath::Max(WalkingSpeed, RunningSpeed);

		// We can't run and crouch at the same time!
		if (IsCrouching()) {
			UnCrouch();
		}
	}
	else {
		MaxWalkSpeed = WalkingSpeed;
	}
}

float UIntruderMovementComponent::GetStamina()
{
	return stamina;
}