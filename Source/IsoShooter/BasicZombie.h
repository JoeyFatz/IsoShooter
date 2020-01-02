// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicZombie.generated.h"

UCLASS()
class ISOSHOOTER_API ABasicZombie : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicZombie();

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* SkeletalMesh;

	class UCapsuleComponent* CapsuleCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | Damage")
	class USoundCue* BulletHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds | Damage")
	USoundCue* DamageGruntSound;

	float Health;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** This is the function that is called when the taget's health falls below zero */
	void Die();

	/** This function takes a value and damages the target with that value 
	/* @param Damage is the amount to damage the target by
	*/
	void DamageEnemy(float Damage);
};
