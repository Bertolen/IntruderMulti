// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IntruderMulti/Actors/Usable.h"
#include "ValuableItem.generated.h"

UCLASS()
class INTRUDERMULTI_API AValuableItem : public AUsable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AValuableItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// This function will be called when the user uses the object
	virtual void OnUsed(ACharacter* User) override;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastDestroy();

	UFUNCTION(Server, Reliable, WithValidation)
		void ReturnToSpawnTransform();

	////// Getters

	FORCEINLINE FTransform GetSpawnTransform() const { return SpawnTransform; }

	FORCEINLINE USoundWave * GetTakenSound() const { return TakenSound; }

	////// Setters

	FORCEINLINE void SetSpawnTransform(const FTransform _SpawnTransform) { SpawnTransform = _SpawnTransform; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FTransform SpawnTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USoundWave* TakenSound;	
};
