// Fill out your copyright notice in the Description page of Project Settings.

#include "Thief.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "IntruderMulti/Characters/FP_Characters/Guard.h"
#include "IntruderMulti/GameMode/GameplayGM.h"
#include "IntruderMulti/PlayerController/GameplayPC.h"

AThief::AThief(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Constructor AThief - Begin"));

	// Set a rotation when the controller rotates so the model is always facing the same direction as the controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	GetCameraComponent()->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	GetCameraComponent()->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset (to avoid direct content references in C++)
	
	// Thieves can crouch
	GetMovementComponent()->NavAgentProps.bCanCrouch = true;

	// Init some values
	UsingReach += CameraBoom->TargetArmLength;
	bIsCarryingAValuable = false;
	RespawnTime = 10.0f;

	// Thieves can't run, they are always at max speed
	GetCharacterMovement()->MaxWalkSpeed *= SpeedMultiplier;
	SpeedMultiplier = 1.0f;

	UE_LOG(IntruderDebug, Verbose, TEXT("Constructor AThief - End"));
}

void AThief::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AThief, bIsCarryingAValuable);
	DOREPLIFETIME(AThief, ValuableClass);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThief::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AThief::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThief::MoveRight);
}

void AThief::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AThief::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

////////////////////////////
// Usable interface

// This function can be called to know if the object can be used or not by the given character
bool AThief::CanBeUsed(ACharacter* User)
{
	AGuard* Guard = Cast<AGuard>(User);
	if (Guard) {
		return true;
	}
	return false;
}

// This function will be called when the user uses the object
void AThief::OnUsed(ACharacter* User)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("OnUsed - Begin"));

	AGuard* Guard = Cast<AGuard>(User);
	if (!Guard) {
		return;
	}

	Guard->Capture(this);

	UE_LOG(IntruderDebug, Verbose, TEXT("OnUsed - End"));
}
////////////////////////

bool AThief::GrabAValuable_Validate(AValuableItem* Item)
{
	return true;
}

void AThief::GrabAValuable_Implementation(AValuableItem* Item)
{
	SetIsCarryingAValuable(true);
	SetValuableClass(Item->GetClass());
	ValuableSpawnTransform = Item->GetSpawnTransform();
	
	Item->MulticastDestroy();
}

bool AThief::GotCaught_Validate(class AGuard* Catcher)
{
	return true;
}

void AThief::GotCaught_Implementation(class AGuard* Catcher)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("GotCaught_Implementation - Begin"));

	if (bIsCarryingAValuable) {
		// Spawn the valuable at the thief's location
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Spawning %s actor"), *GetValuableClass().GetDefaultObject()->GetFName().ToString()));
		AValuableItem * Valuable = GetWorld()->SpawnActor<AValuableItem>(GetValuableClass(), GetActorTransform());
		Valuable->SetCanBeUsedByGuard(true);
		Valuable->SetSpawnTransform(ValuableSpawnTransform);
	}

	// Inform the game state there's one less thief
	AGameplayGM * GameplayGM = Cast<AGameplayGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameplayGM) {
		GameplayGM->OneLessThief();
	}

	// kill the pawn
	OnDeath();

	// Set the time to respawn
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AThief::RespawnPlayer, RespawnTime);

	UE_LOG(IntruderDebug, Verbose, TEXT("GotCaught_Implementation - End"));
}

bool AThief::RespawnPlayer_Validate()
{
	return true;
}

void AThief::RespawnPlayer_Implementation()
{
	AGameplayPC * GameplayPC = Cast<AGameplayPC>(GetController());
	if (GameplayPC) {
		GameplayPC->PassCharacterInfoToServer();
	}
}