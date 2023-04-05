// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "DamageTaker.h"

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
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->Start();
		}

	}
	else if (CannonType == ECannonType::AutoProjectile) {
		StartAutofire();
	}
	else
	{
		LaserShot();
	}

	AmmoAmount--;
	GEngine->AddOnScreenDebugMessage(21, 2, FColor::Purple, FString::Printf(TEXT("Ammo left %i"), AmmoAmount));
	
	ReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::LaserShot()
{
	GEngine->AddOnScreenDebugMessage(20, 2, FColor::Green, "Fire - trace");

	FHitResult hitResult;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.AddIgnoredActor(this);
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;


	FVector start = ProjectileSpawnPoint->GetComponentLocation();
	FVector end = start + ProjectileSpawnPoint->GetForwardVector() * FireRange;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
	{
		DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
		if (hitResult.GetActor())
		{
			//AActor* OverlappedActor = hitResult.GetActor();
			//UE_LOG(LogTemp, Warning, TEXT("Ray collided with %s. "), *OverlappedActor->GetName());
			//OverlappedActor->Destroy();

			float Damage = 5.0f;

			AActor* owner = GetOwner();
			IDamageTaker* DamageActor = Cast<IDamageTaker>(hitResult.GetActor());

			if (DamageActor)
			{
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = owner;
				damageData.DamageMaker = this;
				DamageActor->TakeDamage(damageData);
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Yellow, false, 0.5f, 0, 5);
	}
}

void ACannon::Autofire()
{
	if (BurstCount > 0)
	{

		GEngine->AddOnScreenDebugMessage(BurstCount, 2, FColor::Green, "Autofire - projectile burst shot");
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->Start();
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

void ACannon::SetAmmoAmount(int ammo)
{
	AmmoAmount = ammo;
}

int ACannon::GetAmmoAmount()
{
	return AmmoAmount;
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

