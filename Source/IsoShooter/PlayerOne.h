// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerOne.generated.h"

UCLASS()
class ISOSHOOTER_API APlayerOne : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerOne();

	/// Class Calls

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Sounds")
	class USoundCue* GunShotSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	class AWeaponPickup* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	class APickup* ActiveOverlappingItem;

	FORCEINLINE AWeaponPickup* GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetActiveOverlappingItem(APickup* Pickup) { ActiveOverlappingItem = Pickup; }

	/// Variables

	/** Sets camera rotation speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/* TODO Potential camera pitch (NEEDS CLAMP FUNCTION)
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	//float BaseLookUpRate;
	
	/** Sets character movement speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float WalkSpeed;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// Function Calls

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void Raycast();

	bool bRMBDown;
	
	void RMBDown();
	
	void RMBUp();
	
	void SetEquippedWeapon(AWeaponPickup* WeaponToSet);


	// TODO (NEEDS CLAMP) void LookUpRate(float Rate);

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
