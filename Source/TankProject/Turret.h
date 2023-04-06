// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.h"
#include "DamageTaker.h"
#include "GameUnit.h"
#include "Turret.generated.h"


class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKPROJECT_API ATurret : public AGameUnit
{
	GENERATED_BODY()
	
public:	
	ATurret();

	UPROPERTY()
	AGameUnit* TurretPawn;

	//UFUNCTION()
	//void TakeDamage(FDamageData DamageData);

	//UFUNCTION()
	//void Die();

	//UFUNCTION()
	//void DamageTaked(float DamageValue);


	FVector GetEyesPosition() const;

protected:
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//UStaticMeshComponent* BodyMesh;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//UStaticMeshComponent* TurretMesh;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//UArrowComponent * CannonSetupPoint;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//UBoxComponent * BoxCollision;
	
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//class UHealthComponent * HealthComponent;

	//UPROPERTY()
	//ACannon* Cannon;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	//TSubclassOf<ACannon> CannonClass;

	UPROPERTY()
	class APawn * PlayerPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.005f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float CannonSwapSpeed = 5;

	FTimerHandle CannonSwapTimer;

	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";
		
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void SetupCannon(TSubclassOf<ACannon> newCannonClass);
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	void Fire();
	void SwapCannon();

	bool IsPlayerSeen();
};
