// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicZombie.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasicZombie::ABasicZombie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	CapsuleCollider->SetupAttachment(GetRootComponent());
	CapsuleCollider->InitCapsuleSize(32.f, 91.f);
	CapsuleCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CapsuleCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	Health = 100.f;

}

// Called when the game starts or when spawned
void ABasicZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicZombie::Die()
{
	// TODO Add additional death effects/sounds in this function
	Destroy();
}

void ABasicZombie::DamageEnemy(float Damage)
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