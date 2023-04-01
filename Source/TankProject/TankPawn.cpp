// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement and rotation
	MoveTank(DeltaTime);

	//Turret Rotation
	RotateTurret();
}

void ATankPawn::MoveForward(float Value)
{
	targetForwardAxisValue = Value;
}
void ATankPawn::MoveRight(float Value)
{
	targetRightAxisValue = Value;
}

void ATankPawn::RotateRight(float Value)
{
	targetRotateRightAxisValue = Value;
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::Autofire()
{
	if (Cannon)
	{
		Cannon->StartAutofire();
	}
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newCannonClass)
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
}

void ATankPawn::SwapCannon()
{
	int tempAmmo;
	if (Cannon)
	{
		tempAmmo = Cannon->GetAmmoAmount();
	}
	SetupCannon(SecondCannonClass);
	GEngine->AddOnScreenDebugMessage(11, 2, FColor::Purple, FString::Printf(TEXT("Ammo left %i"), tempAmmo));
	Cannon->SetAmmoAmount(tempAmmo);
	TSubclassOf<ACannon> TempCannonClass = MainCannonClass;
	MainCannonClass = SecondCannonClass;
	SecondCannonClass = TempCannonClass;
}

void ATankPawn::AddAmmo(int ammo)
{
	if (Cannon)
	{
		Cannon->SetAmmoAmount(Cannon->GetAmmoAmount() + ammo);
	}
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon(MainCannonClass);
}

void ATankPawn::MoveTank(float DeltaTime)
{
	//Movement
	FVector CurrentPosition = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector movePosition = CurrentPosition + (ForwardVector * targetForwardAxisValue + RightVector * targetRightAxisValue) * MoveSpeed * DeltaTime;
	SetActorLocation(movePosition, true);
	//Rotation
	float yawRotation = RotationSpeed * targetRotateRightAxisValue * DeltaTime;
	FRotator currentRotation = GetActorRotation();
	yawRotation += currentRotation.Yaw;
	FRotator newRotation = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotation);
}

void ATankPawn::RotateTurret()
{
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePosition();
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		FRotator currRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = currRotation.Pitch;
		targetRotation.Roll = currRotation.Roll;

		FRotator newTurretRotation = FMath::Lerp(currRotation, targetRotation, TurretRotationInterpolationKey);
		TurretMesh->SetWorldRotation(newTurretRotation);
	}
}
