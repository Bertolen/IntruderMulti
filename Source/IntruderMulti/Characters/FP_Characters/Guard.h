// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Guard.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API AGuard : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	AGuard(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Called when the guard catches a thief **/
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Capture(class AThief* Thief);

	/** Called when the guard uses a valuable object **/
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void UseValuable(class AValuableItem* Item);

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

};
