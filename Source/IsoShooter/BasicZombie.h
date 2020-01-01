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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
