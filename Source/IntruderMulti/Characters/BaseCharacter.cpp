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

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	RootComponent = GetCapsuleComponent();

	// Create a follow camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	// Set some default values
	UsingReach = 200.0f;

	UE_LOG(IntruderDebug, Verbose, TEXT("Constructor ABaseCharacter - End"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

	// Bind special actions
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ABaseCharacter::Use);

	// Bind UI action
	PlayerInputComponent->BindAction("ToggleDisplayMenus", IE_Pressed, this, &ABaseCharacter::ToggleDisplay);
	PlayerInputComponent->BindAction("ShowInGameMenu", IE_Pressed, this, &ABaseCharacter::ShowGameplayMenu);
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
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, TraceParams);

	// we cast the hit actor to the AUsable Actor
	FocusedUsable = Cast<AUsable>(Hit.GetActor());
}

void ABaseCharacter::Use()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("Use - Begin"));

	// we access the usable item, make sure we have one, else we will crash
	if (FocusedUsable == NULL) {
		return;
	}
	UE_LOG(IntruderDebug, Verbose, TEXT("plop 1"));

	// Use the object, if it can be used.
	if (FocusedUsable->CanBeUsed(this)) {
		UE_LOG(IntruderDebug, Verbose, TEXT("plop 2"));
		FocusedUsable->OnUsed(this);
	}

	UE_LOG(IntruderDebug, Verbose, TEXT("Use - End"));
}

void ABaseCharacter::ShowGameplayMenu()
{
	AGameplayPC* GameplayPC = Cast<AGameplayPC>(GetController());
	if (!GameplayPC) {
		return;
	}

	GameplayPC->ShowMenuWindow();
}

void ABaseCharacter::ToggleDisplay()
{
	UE_LOG(IntruderDebug, Verbose, TEXT("ToggleDisplay - Begin"));
	
	AGameplayPC* GameplayPC = Cast<AGameplayPC>(GetController());
	if (!GameplayPC) {
		return;
	}

	GameplayPC->ToggleDisplay();

	UE_LOG(IntruderDebug, Verbose, TEXT("ToggleDisplay - End"));
}