// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "GameFramework/Character.h"
#include "IntruderMulti/Interface/UsableInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class INTRUDERMULTI_API ABaseCharacter : public ACharacter, public IUsableInterface
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

	UFUNCTION()
	void StartRunning();

	UFUNCTION()
	void StopRunning();

	UFUNCTION()
	void ToggleCrouch();

	/////////////////////////////
	// UsableInterface

	// This function can be called to know if the object can be used or not by the given character
	virtual bool CanBeUsed(ACharacter* User) override;

	// This function will be called when the user uses the object
	virtual void OnUsed(ACharacter* User) override;

	////////////////////////////

	////// Getters
	FORCEINLINE IUsableInterface* GetFocusedUsable() const { return FocusedUsable; }

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	FORCEINLINE float GetStamina() const { return Stamina; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void OnDeath();

	virtual void SetRagdollPhysics();

	void TypeChatMessage();

	/** Updates the focus line and sets the new focused usable item**/
	virtual void UpdateFocusLine();

	/** Updates the stamina according to the character's curent state**/
	virtual void UpdateStamina();

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

	// Whether or not the character can run faster than his base walking speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		bool bCanRun;

	// current desired speed ratio, 1 is max speed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float SpeedMultiplier;

private:
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	// Usable item on focus
	IUsableInterface* FocusedUsable;

	// Does the character wants to run
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		bool bIsRunning;

	float Stamina;
};
