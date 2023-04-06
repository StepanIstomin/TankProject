// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "HealthComponent.h"

ATurret::ATurret()
{
 	PrimaryActorTick.bCanEverTick = false;

	//BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	//RootComponent = BoxCollision;

	//BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
	//BodyMesh->SetupAttachment(BoxCollision);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
	TurretMesh->SetupAttachment(BodyMesh, "ADD_Parts_Here_Socket");

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh,FAttachmentTransformRules::KeepRelativeTransform);
	
	UStaticMesh * turretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (turretMeshTemp)
		TurretMesh->SetStaticMesh(turretMeshTemp);
	UStaticMesh* bodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (bodyMeshTemp)
		BodyMesh->SetStaticMesh(bodyMeshTemp);

	//HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	//HealthComponent->OnDie.AddUObject(this, &ATurret::Die);
	//HealthComponent->OnDamaged.AddUObject(this, &ATurret::DamageTaked);

	//TurretPawn = Cast<AGameUnit>(GetPawn());

}

FVector ATurret::GetEyesPosition() const
{
	return CannonSetupPoint->GetComponentLocation();
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon(MainCannonClass);
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}
void ATurret::Destroyed()
{
	if (Cannon)
		Cannon->Destroy();
}

void ATurret::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if (!newCannonClass)
	{
		return;
	}
	if (Cannon)
	{
		Cannon->Destroy();
	}
	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(newCannonClass, spawnParams);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	GetWorld()->GetTimerManager().SetTimer(CannonSwapTimer, this, &ATurret::SwapCannon, CannonSwapSpeed, true, CannonSwapSpeed);
}

void ATurret::Targeting()
{
	if (!PlayerPawn || !IsPlayerSeen())
	{
		return;
	}
	if (IsPlayerInRange())
	{
		RotateToPlayer();
		if (CanFire())
		{
			Fire();
		}
	}
}
void ATurret::RotateToPlayer()
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
	PlayerPawn->GetActorLocation());

	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));
}
bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire()
{
	FVector targetingDir = TurretMesh->GetForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));
	return aimAngle <= Accurency;
}
void ATurret::Fire()
{
	if (Cannon) {
		GEngine->AddOnScreenDebugMessage(40, 2, FColor::Green, "Turret fire");
		Cannon->Fire();
	}
		
}

void ATurret::SwapCannon()
{
	SetupCannon(SecondCannonClass);
	TSubclassOf<ACannon> TempCannonClass = MainCannonClass;
	MainCannonClass = SecondCannonClass;
	SecondCannonClass = TempCannonClass;
}

bool ATurret::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = GetEyesPosition();
	FHitResult hitResult;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	//traceParams.AddIgnoredActor(TurretPawn);
	traceParams.bReturnPhysicalMaterial = false;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, traceParams))
	{
		if (hitResult.GetActor())
		{
			if (hitResult.GetActor() == PlayerPawn)
			{
				//DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Red, false, 0.1f, 0, 10);
			}
			//DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Cyan, false, 0.1f, 0, 10);
			return hitResult.GetActor() == PlayerPawn;
		}
	}
	//DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Red, false, 0.1f, 0, 10);
	return false;
}
