// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Exit.generated.h"

UCLASS()
class INTRUDERMULTI_API AExit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	////// Getters
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

protected:
	// The trigger space inside which if a thief holding the valuable enters he'll win the game
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* ExitVolume;

private:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent * Mesh;	
};
