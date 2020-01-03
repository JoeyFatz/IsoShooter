// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "PlayerOne.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(800.f);

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	AttackSphere->SetupAttachment(GetRootComponent());
	AttackSphere->InitSphereRadius(75.f);

	bOverlappingAttackSphere = false;

	Health = 100.f;
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
	// References Artificial Controller
	AIController = Cast<AAIController>(GetController());

	// Binds functions to components
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AZombie::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AZombie::AgroSphereOnOverlapEnd);

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AZombie::AttackSphereOnOverlapBegin);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AZombie::AttackSphereOnOverlapEnd);
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombie::Die()
{
	// TODO Add additional death effects/sounds in this function
	Destroy();
}

void AZombie::DamageEnemy(float Damage)
{
	// Damage is 34.f from PlayerOne.cpp
	Health -= Damage;

	UGameplayStatics::PlaySound2D(this, DamageGruntSound);
	UGameplayStatics::PlaySound2D(this, BulletHitSound);


	// TODO Add additional death effects/sounds in this function

	if (Health <= 0)
	{
		Die();
	}
}

// AgroSphere Overlap functions
void AZombie::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		APlayerOne* Player = Cast<APlayerOne>(OtherActor);
		if (Player)
		{
			MoveToTarget(Player);
		}
	}
}
void AZombie::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerOne* Player = Cast<APlayerOne>(OtherActor);
		{
			if (Player)
			{
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
				if (AIController)
				{
					AIController->StopMovement();
				}
			}
		}
	}
}

// CombatSphere Overlap functions
void AZombie::AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		APlayerOne* Player = Cast<APlayerOne>(OtherActor);
		{
			if (Player)
			{
				AttackTarget = Player;
				bOverlappingAttackSphere = true;
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
			}
		}
	}
}
void AZombie::AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerOne* Player = Cast<APlayerOne>(OtherActor);
		{
			if (Player)
			{
				bOverlappingAttackSphere = false;
				if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
				{
					MoveToTarget(Player); // Calling when animation is finished in Blueprints
					AttackTarget = nullptr;
				}
			}
		}
	}
}

void AZombie::MoveToTarget(class APlayerOne* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		FAIMoveRequest MoveRequest; // Access MoveRequest Struct
		MoveRequest.SetGoalActor(Target); // Sets Actor to follow
		MoveRequest.SetAcceptanceRadius(10.0); // Sets the distance between the character capsules before collision 

		FNavPathSharedPtr NavPath; // Out Path (Has no initial value)

		AIController->MoveTo(MoveRequest, &NavPath);

		/** (Visualize Nav Path)
		auto PathPoints = NavPath->GetPathPoints(); // <TArray> FNavPathPoint
		for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
		}
		*/
	}
}
