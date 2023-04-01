// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonBox.h"
#include "TankPawn.h"

// Sets default values
ACannonBox::ACannonBox()
{
	PrimaryActorTick.bCanEverTick = true;

    USceneComponent* SceneCopm = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = SceneCopm;

    CannonBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonBoxMesh"));
    CannonBoxMesh->SetupAttachment(RootComponent);
    CannonBoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CannonBoxMesh->SetCollisionProfileName("OverlapAll");
    CannonBoxMesh->OnComponentBeginOverlap.AddDynamic(this, &ACannonBox::OnMeshOverlapBegin);
}

void ACannonBox::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
    if (TankPawn)
    {
        TankPawn->SetupCannon(CannonClass);
        Destroy();
    }
}

