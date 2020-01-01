// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerOne.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
APlayerOne::APlayerOne()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/// Character collision capsule

	GetCapsuleComponent()->InitCapsuleSize(32.f, 91.f);

	/// Sets Player Camera and Values

	FVector CameraHeight = FVector(0.f, 0.f, 700.f);
	FRotator CameraRotation = FRotator(-45.f, 0.f, 0.f);

	// Spring Arm Component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetRelativeLocation(CameraHeight);
	CameraBoom->TargetArmLength = 700.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 3.f;
	// TODO Calculate max camera lag distance

	// Camera Component
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeRotation(CameraRotation);
	FollowCamera->bUsePawnControlRotation = false;
	
	// Camera rotation speed
	BaseTurnRate = 65.f;

	/// Sets Character Movement and Values

	// Character movement speed
	WalkSpeed = 400.f;

	// Sets character rotation to movement (Independent of camera)
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

}

// Called when the game starts or when spawned
void APlayerOne::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Called every frame
void APlayerOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerOne::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	/// Action Mappings

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &APlayerOne::Raycast);

	/// Axis Mappings

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerOne::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerOne::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerOne::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerOne::TurnAtRate);
	
	// TODO (NEEDS CLAMP) PlayerInputComponent->BindAxis("LookUp", this, &APlayerOne::AddControllerPitchInput);
	// TODO (NEEDS CLAMP) PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerOne::LookUpRate);
	
}

void APlayerOne::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerOne::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APlayerOne::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// TODO (NEEDS CLAMP)
/*
void APlayerOne::LookUpRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
*/

void APlayerOne::Raycast()
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = GetCapsuleComponent()->GetComponentLocation();
	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	FVector EndTrace = (ForwardVector * 1000.f) + StartTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *CQP))
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);

		if (HitResult->GetActor() != NULL)
		{
			HitResult->GetActor()->Destroy();
		}
	}
}
