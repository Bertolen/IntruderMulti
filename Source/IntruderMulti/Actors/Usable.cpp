// Fill out your copyright notice in the Description page of Project Settings.

#include "Usable.h"
#include "IntruderMulti/Characters/FP_Characters/Guard.h"
#include "IntruderMulti/Characters/TP_Characters/Thief.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AUsable::AUsable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call never Tick().
	PrimaryActorTick.bCanEverTick = false;

	// Init the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
}

// Called when the game starts or when spawned
void AUsable::BeginPlay()
{
	Super::BeginPlay();

}

// This function can be called to know if the object can be used or not by th given controller
bool AUsable::CanBeUsed(ACharacter* User)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("CanBeUsed - Begin"));

	AGuard * isGuard = Cast<AGuard>(User);
	if (isGuard) {
		if (bCanBeUsedByGuard) {
			UE_LOG(IntruderDebug, Verbose, TEXT("CanBeUsed - true"));
			return true;
		}
	}

	AThief * isThief = Cast<AThief>(User);
	if (isThief) {
		if (bCanBeUsedByThief) {
			UE_LOG(IntruderDebug, Verbose, TEXT("CanBeUsed - true"));
			return true;
		}
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("CanBeUsed - false"));
	return false;
}

// This function will be called when the user uses the object
void AUsable::OnUsed(ACharacter* Newuser)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("OnUsed - Begin"));

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Usable object used")));

	UE_LOG(IntruderDebug, Verbose, TEXT("OnUsed - End"));
}

////// Setters
bool AUsable::SetCanBeUsedByThief_Validate(const bool NewValue)
{
	return true;
}

void AUsable::SetCanBeUsedByThief_Implementation(const bool NewValue)
{
	bCanBeUsedByThief = NewValue;
}

bool AUsable::SetCanBeUsedByGuard_Validate(const bool NewValue)
{
	return true;
}

void AUsable::SetCanBeUsedByGuard_Implementation(const bool NewValue)
{
	bCanBeUsedByGuard = NewValue;
}