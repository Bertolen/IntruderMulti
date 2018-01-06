// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "GameFramework/Character.h"
#include "IntruderMulti/Actors/Usable.h"
#include "BaseCharacter.generated.h"

UCLASS()
class INTRUDERMULTI_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Handles moving forward/backward */
	virtual void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	virtual void MoveRight(float Val);

	/** Updates the focus line and sets the new focused usable item**/
	virtual void UpdateFocusLine();

	////// Getters
	FORCEINLINE AUsable* GetFocusedUsable() const { return FocusedUsable; }

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// This method uses the usable item on focus
	UFUNCTION(BlueprintCallable, Category = "Hability")
	virtual void Use();

	UFUNCTION()
	virtual void ShowGameplayMenu();

protected:
	// maximum distance at which the character can use an item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hability", meta = (AllowPrivateAccess = "true"))
		float UsingReach;

private:
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	// Usable item on focus
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hability", meta = (AllowPrivateAccess = "true"))
	AUsable* FocusedUsable;
	
};
