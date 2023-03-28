// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

	BurstCount = ShotsInBurst;
}

void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		return;
	}
	
	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(20, 2, FColor::Green, "Fire - projectile");

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(20, 2, FColor::Green, "Fire - trace");
	}

	AmmoAmount--;
	GEngine->AddOnScreenDebugMessage(21, 2, FColor::Purple, FString::Printf(TEXT("Ammo left %i"), AmmoAmount));
	
	ReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::Autofire()
{
	if (BurstCount > 0)
	{
		if (CannonType == ECannonType::FireProjectile)
		{
			GEngine->AddOnScreenDebugMessage(BurstCount, 2, FColor::Green, "Autofire - projectile burst shot");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(BurstCount, 2, FColor::Green, "Autofire - trace burst shot");
		}
		BurstCount--;
		BurstActive = true;
		GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &ACannon::Autofire, 1 / BurstRate, false);
	}
	else
	{
		BurstActive = false;
		BurstCount = ShotsInBurst;
		AmmoAmount--;
		GEngine->AddOnScreenDebugMessage(11, 2, FColor::Purple, FString::Printf(TEXT("Ammo left %i"), AmmoAmount));
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
	}
}

void ACannon::StartAutofire()
{
	if (!IsReadyToFire() || BurstActive)
	{
		return;
	}
	ReadyToFire = false;
	Autofire();
}

bool ACannon::IsReadyToFire()
{
	if (AmmoAmount <= 0) {
		GEngine->AddOnScreenDebugMessage(13, 2, FColor::Red, "Ammo is empty");
	}
	return ReadyToFire;
}


// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

void ACannon::Reload()
{
	if (AmmoAmount > 0)
	{
		ReadyToFire = true;
	}
}

