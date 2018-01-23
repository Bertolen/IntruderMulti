// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "IntruderMulti/Actors/ValuableItem.h"
#include "Thief.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API AThief : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	AThief(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Handles moving forward/backward */
	virtual void MoveForward(float Val) override;

	/** Handles stafing movement, left and right */
	virtual void MoveRight(float Val) override;

	/** Called when the thief grabs a valuable object **/
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void GrabAValuable(class AValuableItem* Item);

	/** Called when the thief got caught by a guard **/
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void GotCaught(class AGuard* Catcher);

	/////////////////////////////
	// UsableInterface

	// This function can be called to know if the object can be used or not by the given character
	virtual bool CanBeUsed(ACharacter* User) override;

	// This function will be called when the user uses the object
	virtual void OnUsed(ACharacter* User) override;

	////////////////////////////

	////// Getters

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	FORCEINLINE bool GetIsCarryingAValuable() const { return bIsCarryingAValuable; }

	FORCEINLINE TSubclassOf<AValuableItem> GetValuableClass() const { return ValuableClass; }

	////// Setters

	void SetIsCarryingAValuable(const bool NewValue) { bIsCarryingAValuable = NewValue; }

	void SetValuableClass(TSubclassOf<AValuableItem> NewValuableClass) { ValuableClass = NewValuableClass; }

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsCarryingAValuable;

	UPROPERTY(Replicated)
	TSubclassOf<AValuableItem> ValuableClass;

	UPROPERTY()
	FTransform ValuableSpawnTransform;
};
