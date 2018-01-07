// Fill out your copyright notice in the Description page of Project Settings.

#include "ValuableItem.h"
#include "Components/StaticMeshComponent.h"
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

	AThief* Thief = Cast<AThief>(Newuser);
	if (Thief) {
		// TODO (this code is temporary)
		ThievesWin();
	}
	else {
		AGuard* Guard = Cast<AGuard>(Newuser);
		if (Guard) {
			// TODO
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