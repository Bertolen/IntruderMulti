// Fill out your copyright notice in the Description page of Project Settings.

#include "ValuableItem.h"
#include "IntruderMulti/Characters/FP_Characters/Guard.h"
#include "IntruderMulti/Characters/TP_Characters/Thief.h"
#include "IntruderMulti/GameMode/GameplayGM.h"

// Sets default values
AValuableItem::AValuableItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to be able to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Init the mesh
	if (GetMesh()) {
		GetMesh()->SetSimulatePhysics(true);
	}

	// By default this item can't be used by a guard, it can only be stolen by a thief
	bCanBeUsedByGuard = false;
	bCanBeUsedByThief = true;
}

// Called when the game starts or when spawned
void AValuableItem::BeginPlay()
{
	Super::BeginPlay();
	
	// This actor can't call Tick yet
	SetActorTickEnabled(false);

	// Init the SpawnTransform
	SpawnTransform = GetActorTransform();
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

	AThief* Thief = Cast<AThief>(Newuser);
	if (Thief) {
		// The item is removed from the level and the thief memorize he has it
		Thief->GrabAValuable(this);
	}
	else {
		AGuard* Guard = Cast<AGuard>(Newuser);
		if (Guard) {
			Guard->UseValuable(this);
		}
	}
}

void AValuableItem::MulticastDestroy_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "MulticastDestroy");

	UGameplayStatics::SpawnSoundAttached(GetTakenSound(), GetRootComponent());
	Destroy();
}

bool AValuableItem::ReturnToSpawnTransform_Validate()
{
	return true;
}

void AValuableItem::ReturnToSpawnTransform_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Return to spawn transform"));
	SetActorTransform(SpawnTransform);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}