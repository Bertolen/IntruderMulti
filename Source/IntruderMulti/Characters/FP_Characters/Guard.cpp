// Fill out your copyright notice in the Description page of Project Settings.

#include "Guard.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "IntruderMulti/Characters/TP_Characters/Thief.h"

AGuard::AGuard(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Constructor AGuard - Begin"));

	// Create a CameraComponent	
	GetCameraComponent()->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	GetCameraComponent()->SetupAttachment(RootComponent);
	GetCameraComponent()->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(GetCameraComponent());
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	
	// Hide the main Mesh (that's a 3rd person mesh) to the owner, and show the 1st person Mesh to the owner only
	GetMesh()->bOwnerNoSee = true;
	Mesh1P->bOnlyOwnerSee = true;

	UE_LOG(IntruderDebug, Verbose, TEXT("Constructor AGuard - End"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGuard::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AGuard::Capture_Validate(AThief* Thief)
{
	return true;
}

void AGuard::Capture_Implementation(AThief* Thief)
{
	Thief->GotCaught(this);
}