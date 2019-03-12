// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IntruderMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API UIntruderMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public :
	// Sets default values for this component's properties
	UIntruderMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(Server, Reliable, WithValidation)
	void SetIsRunning(bool newValue);

	float GetStamina();

public:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float WalkingSpeed;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float RunningSpeed;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bCanRun;

private :
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bIsRunning;

	float stamina;
};
