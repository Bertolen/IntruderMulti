// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
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

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

};
