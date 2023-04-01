// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "TankPawn.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
 	PrimaryActorTick.bCanEverTick = true;

    USceneComponent* SceneCopm = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = SceneCopm;

    AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
    AmmoMesh->SetupAttachment(RootComponent);
    AmmoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    AmmoMesh->SetCollisionProfileName("OverlapAll"); 
    AmmoMesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
}

void AAmmoBox::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
    if (TankPawn)
    {
        TankPawn->AddAmmo(AmmoAmmount);
        Destroy();
    }
}

