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
	Stamina = 100.0f;
	MaxWalkSpeedCrouched = WalkingSpeed / 2;
	MaxWalkSpeed = WalkingSpeed;
}

void UIntruderMovementComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UIntruderMovementComponent, bIsRunning);
}

// TODO
void UIntruderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Checks if the character has been running 
	if (bIsRunning && GetLastUpdateVelocity().Size() > 1e-8f  && IsMovingOnGround()) {

		// if so we're gonna use up stamina
	}
	else {
		// if not we can recover some stamina
	}
}

///////////////////////// ACCESSORS ////////////////////////////

bool UIntruderMovementComponent::SetIsRunning_Validate(bool newValue) 
{
	return true;
}

void UIntruderMovementComponent::SetIsRunning_Implementation(bool newValue) 
{
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
	return Stamina;
}