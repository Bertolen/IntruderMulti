// Fill out your copyright notice in the Description page of Project Settings.

#include "Exit.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "IntruderMulti/Characters/TP_Characters/Thief.h"
#include "IntruderMulti/GameMode/GameplayGM.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExit::AExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	Mesh->SetupAttachment(GetRootComponent());
	//RootComponent = Mesh;

	// Setup the climb volume
	ExitVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitVolume"));
	ExitVolume->SetupAttachment(GetMesh());
	ExitVolume->bGenerateOverlapEvents = true;
	ExitVolume->SetCollisionResponseToAllChannels(ECR_Overlap);
	ExitVolume->OnComponentBeginOverlap.AddDynamic(this, &AExit::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AExit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExit::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("OnOverlapBegin - Begin"));

	AThief* Thief = Cast<AThief>(OtherActor);
	if (Thief && Thief->GetIsCarryingAValuable()) {
		AGameplayGM* GameplayGM = Cast<AGameplayGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (!GameplayGM) {
			return;
		}

		GameplayGM->ThievesWin();
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("OnOverlapBegin - End"));
}