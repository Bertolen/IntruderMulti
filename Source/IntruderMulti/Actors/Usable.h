// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "GameFramework/Actor.h"
#include "IntruderMulti/Interface/UsableInterface.h"
#include "Usable.generated.h"

UCLASS()
class INTRUDERMULTI_API AUsable : public AActor, public IUsableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUsable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// This function can be called to know if the object can be used or not by the given character
	virtual bool CanBeUsed(ACharacter* User) override;

	// This function will be called when the user uses the object
	virtual void OnUsed(ACharacter* User) override;

	////// Getters
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

	////// Setters
	UFUNCTION(Server, Reliable, WithValidation)
	void SetCanBeUsedByThief(const bool NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void SetCanBeUsedByGuard(const bool NewValue);

protected:
	// Indicates if this item can be used by a thief
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanBeUsedByThief;

	// Indicates if this item can be used by a guard
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanBeUsedByGuard;

private:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent * Mesh;
};
