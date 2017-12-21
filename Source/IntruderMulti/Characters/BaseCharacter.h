// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class INTRUDERMULTI_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Handles moving forward/backward */
	virtual void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	virtual void MoveRight(float Val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	
};
