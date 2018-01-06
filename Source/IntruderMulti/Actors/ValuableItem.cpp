// Fill out your copyright notice in the Description page of Project Settings.

#include "ValuableItem.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AValuableItem::AValuableItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to be able to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Init the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	if (Mesh) {
		Mesh->SetSimulatePhysics(false);
	}

	bCanBeUsedByGuard = true;
	bCanBeUsedByThief = true;
}

// Called when the game starts or when spawned
void AValuableItem::BeginPlay()
{
	Super::BeginPlay();
	
	// This actor can't call Tick yet
	SetActorTickEnabled(false);
}

// Called every frame
void AValuableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// This function will be called when the user uses the object
void AValuableItem::OnUsed(ACharacter* Newuser)
{
	Super::OnUsed(Newuser);
}