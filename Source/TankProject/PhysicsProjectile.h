// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"

#include "PhysicsProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TANKPROJECT_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	APhysicsProjectile();

	virtual void Start() override;

protected:
	virtual void Move() override;
	//void Explode();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPhysicsComponent* PhysicsComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent * TrailEffect;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//float ExplodeRadius = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveAccurency = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	float TrajectoryMaxTime = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	float TrajectoryTimeStep = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	float TrajectorySpeed = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	bool ShowTrajectory = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	FVector MoveVector;

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	TArray<FVector> CurrentTrajectory;

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	int32 TragectoryPointIndex;

};
