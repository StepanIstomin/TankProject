// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMapLoader::AMapLoader()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;
	
	ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated lights"));
	ActivatedLight->SetupAttachment(sceneComp);

	DeactivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Deactivated lights"));
	DeactivatedLight->SetupAttachment(sceneComp);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger collider"));
	BoxCollision->SetupAttachment(sceneComp);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this,&AMapLoader::OnTriggerOverlapBegin);

	MapLoaderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gates Mesh"));
	MapLoaderMesh->SetupAttachment(BoxCollision);

	SetActiveLights();
}

void AMapLoader::SetIsActivated(bool NewIsActivated)
{
	IsActivated = NewIsActivated;
	SetActiveLights();
}

void AMapLoader::BeginPlay()
{
	Super::BeginPlay();
	SetActiveLights();
}

void AMapLoader::SetActiveLights()
{
	ActivatedLight->SetHiddenInGame(!IsActivated);
	DeactivatedLight->SetHiddenInGame(IsActivated);

}

void AMapLoader::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsActivated) {
		UE_LOG(LogTemp, Warning, TEXT("Gate is not activated"));
		return;
	}
		
	UE_LOG(LogTemp, Warning, TEXT("Gate triggered phase 1"));
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (OtherActor == PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gate triggered"));
		UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
	}
}
