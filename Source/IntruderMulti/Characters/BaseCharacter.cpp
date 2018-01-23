// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "IntruderMulti/PlayerController/GameplayPC.h"

// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Constructor ABaseCharacter - Begin"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Activate Replication
	bReplicates = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	RootComponent = GetCapsuleComponent();

	// Create a follow camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	// Set some default values
	UsingReach = 200.0f;
	WalkingSpeed = 200.0f;
	RunningSpeed = 600.0f;
	bIsRunning = false;

	UE_LOG(IntruderDebug, Verbose, TEXT("Constructor ABaseCharacter - End"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setup the speed for the editor
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkingSpeed / 2;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// checks if we can use something
	UpdateFocusLine();
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Bind running events
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ABaseCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ABaseCharacter::StopRunning);

	// Bind special actions
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ABaseCharacter::Use);

	// Bind UI action
	PlayerInputComponent->BindAction("ShowInGameMenu", IE_Pressed, this, &ABaseCharacter::ShowGameplayMenu);
	PlayerInputComponent->BindAction("TypeChatMessage", IE_Pressed, this, &ABaseCharacter::TypeChatMessage);
}

void ABaseCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ABaseCharacter::UpdateFocusLine()
{
	FVector CamLoc;
	FRotator CamRot;

	// we check the controller, else we'll crash
	if (!GetController() || !GetController()->IsLocalPlayerController()) {
		return;
	}

	GetController()->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation

	const FVector StartTrace = CamLoc; // trace start is the camera location
	const FVector Direction = CamRot.Vector();
	const FVector EndTrace = StartTrace + Direction * UsingReach; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks

																	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	FHitResult Hit(ForceInit);

	// simple trace function
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_Camera, TraceParams);

	// we cast the hit actor to the IUsableInterface Actor
	FocusedUsable = Cast<IUsableInterface>(Hit.GetActor());
}

void ABaseCharacter::Use()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Use - Begin"));

	// we access the usable item, make sure we have one, else we will crash
	if (FocusedUsable == NULL) {
		return;
	}

	// Use the object, if it can be used.
	if (FocusedUsable->CanBeUsed(this)) {
		FocusedUsable->OnUsed(this);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("Use - End"));
}

void ABaseCharacter::ShowGameplayMenu()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ShowGameplayMenu - Begin"));

	AGameplayPC* GameplayPC = Cast<AGameplayPC>(GetController());
	if (!GameplayPC) {
		return;
	}

	GameplayPC->ShowMenuWindow();

	UE_LOG(IntruderDebug, Verbose, TEXT("ShowGameplayMenu - End"));
}

void ABaseCharacter::TypeChatMessage()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("TypeChatMessage - Begin"));

	AGameplayPC* GameplayPC = Cast<AGameplayPC>(GetController());
	if (!GameplayPC) {
		return;
	}

	GameplayPC->TypeChatMessage();

	UE_LOG(IntruderDebug, Verbose, TEXT("TypeChatMessage - End"));
}

void ABaseCharacter::OnDeath_Implementation()
{
	bReplicateMovement = false;
	bTearOff = true;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	USkeletalMeshComponent* Mesh1P = GetMesh();

	if (Mesh1P)
	{
		Mesh1P->SetOwnerNoSee(false);
		Mesh1P->SetCollisionProfileName(TEXT("Ragdoll"));
	}

	SetActorEnableCollision(true);

	SetRagdollPhysics();
}

void ABaseCharacter::SetRagdollPhysics()
{
	bool bInRagdoll = false;
	USkeletalMeshComponent* Mesh1P = GetMesh();

	if (IsPendingKill())
	{
		bInRagdoll = false;
	}
	else if (!Mesh1P || !Mesh1P->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		Mesh1P->SetAllBodiesSimulatePhysics(true);
		Mesh1P->SetSimulatePhysics(true);
		Mesh1P->WakeAllRigidBodies();
		Mesh1P->bBlendPhysics = true;

		bInRagdoll = true;
	}

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}

	if (!bInRagdoll)
	{
		// Immediately hide the pawn
		TurnOff();
		SetActorHiddenInGame(true);
	}
}

void ABaseCharacter::StartRunning()
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = FMath::Max(WalkingSpeed, RunningSpeed);

	// TODO
	// We can't run and crouch at the same time!
	/*if (bIsCrouched) {
		ToggleCrouch();
	}*/
}

void ABaseCharacter::StopRunning()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

////////////////////////////
// Usable interface

// This function can be called to know if the object can be used or not by the given character
bool ABaseCharacter::CanBeUsed(ACharacter* User)
{
	return false;
}

// This function will be called when the user uses the object
void ABaseCharacter::OnUsed(ACharacter* Newuser)
{

}
////////////////////////