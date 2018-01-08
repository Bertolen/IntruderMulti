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
		GetMesh()->SetSimulatePhysics(false);
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
		Thief->SetIsCarryingAValuable(true);
		Destroy();
	}
	else {
		AGuard* Guard = Cast<AGuard>(Newuser);
		if (Guard) {
			// TODO
			// What to do when a guard uses the item
		}
	}
}

bool AValuableItem::ThievesWin_Validate()
{
	return true;
}

void AValuableItem::ThievesWin_Implementation()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ThievesWin_Implementation - Begin"));

	AGameplayGM* GameplayGM = Cast<AGameplayGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameplayGM) {
		return;
	}

	GameplayGM->ThievesWin();

	UE_LOG(IntruderDebug, Verbose, TEXT("ThievesWin_Implementation - End"));
}