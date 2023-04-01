// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKPROJECT_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateRight(float Value);

	void Fire();
	void Autofire();

	void SetupCannon(TSubclassOf<ACannon> newCannonClass);
	void SetupSecondCannon(TSubclassOf<ACannon> newCannonClass);
	void SwapCannon();
	void AddAmmo(int ammo);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm; // component for camera

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY()
	ACannon* Cannon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> MainCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> SecondCannonClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	class UArrowComponent* CannonSetupPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Rotation")
	float TurretRotationInterpolationKey = 0.5f;

	UPROPERTY()
	class ATankPlayerController* TankController;

private:
	float targetForwardAxisValue = 0.0f;
	float targetRightAxisValue = 0.0f;
	float targetRotateRightAxisValue = 0.0f;

	void MoveTank(float Deltatime);

	void RotateTurret();
};
