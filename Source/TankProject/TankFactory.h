// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "TankPawn.h"
#include "MapLoader.h"
#include "TankFactory.generated.h"

UCLASS()
class TANKPROJECT_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	ATankFactory();

	UFUNCTION()
	void TakeDamage(FDamageData DamageData);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* DestroyedMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent * TankSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent * HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent * HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<ATankPawn> SpawnTankClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
	float SpawnTankRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TArray<ATargetPoint*> TankWayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	class AMapLoader* LinkedMapLoader;

	bool bNotDestoyed = true;
	
protected:
	virtual void BeginPlay() override;

	void SpawnNewTank();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);
};
