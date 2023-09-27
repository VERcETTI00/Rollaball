// Fill out your copyright notice in the Description page of Project Settings.


#include "RollaBall/Game/RollaBallPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ARollaBallPlayer::ARollaBallPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	// Set root component to the mesh
	RootComponent = Mesh;
	// Attach the SpringArm to the Mesh, Spring will now follow the Mesh transform.
	SpringArm -> SetupAttachment(Mesh);
	// Attach the Camera to the SpringArm, Camera will now follow the SpringArm transform.
	Camera->SetupAttachment(SpringArm);

	// Set Physics to true
	Mesh->SetSimulatePhysics(true);

	Mesh->OnComponentHit.AddDynamic(this, &ARollaBallPlayer::OnHit);
}

// Called when the game starts or when spawned
void ARollaBallPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Account for mass in MoveForce
	MoveForce *= Mesh->GetMass();
	JumpImpulse *= Mesh->GetMass();
}

// Called to bind functionality to input
void ARollaBallPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Custom Input Axis Binding
	InputComponent->BindAxis("MoveForward", this, &ARollaBallPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARollaBallPlayer::MoveRight);
	// Custom Action Binding
	InputComponent->BindAction("Jump", IE_Pressed, this, &ARollaBallPlayer::Jump);

}

void ARollaBallPlayer::MoveRight(float Value)
{
	// Get the Right Vector of the Camera as it doesn't rotate and move the player in this direction based on the input and MoveForce
	const FVector Right = Camera->GetRightVector() * MoveForce * Value;
	Mesh->AddForce(Right);
}

void ARollaBallPlayer::MoveForward(float Value)
{
	const FVector Forward = Camera->GetForwardVector() * MoveForce * Value;
	Mesh->AddForce(Forward);
}

void ARollaBallPlayer::Jump()
{
	// Cap the no of jumps player can make
	if (JumpCount >= MaxJumpCount) { return; }
	// Apply an Impulse to the Mesh in the Z axis
	Mesh->AddImpulse(FVector(0, 0, JumpImpulse));
	// Track how many times we've jumped
	JumpCount++;
}

void ARollaBallPlayer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Get Direction we hit the surface on Z axis
	const float HitDirection = Hit.Normal.Z;
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, FString::Printf(TEXT("Z Normal : %f"), HitDirection));*/
	// If it's more than 0 then we've hit something below us. 1 is flat, anything between is slope
	if (HitDirection > 0) {
		JumpCount = 0;
	}
}
