// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "DamageTaker.h"

// Sets default values
AProjectile::AProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneCopm = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneCopm;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SphereCollision);
}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &AProjectile::Move, MoveRate, true, 0);
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	if (ExplodingProjectile)
	{
		Explode();
	}
	else
	{
		PushAndDamage(OtherActor);
	}
	Destroy();
}

void AProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;
	FQuat Rotation = FQuat::Identity;
	bool sweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackHit,
		startPos,
		endPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		params
	);
	GetWorld()->DebugDrawTraceTag = "Explode Trace";
	if (sweepResult)
	{
		for (FHitResult hitResult : AttackHit)
		{
			AActor* OtherActor = hitResult.GetActor();

			if (!OtherActor)
				continue;

			PushAndDamage(OtherActor);
		}
	}
}

void AProjectile::PushAndDamage(AActor* OtherActor)
{
	AActor* owner = GetOwner(); // Cannon
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr; // Tank or Turret
	AActor* ownerByOwnerByOwner = ownerByOwner != nullptr ? ownerByOwner->GetOwner() : nullptr; // root

	if (OtherActor != owner && OtherActor != ownerByOwner && OtherActor != ownerByOwnerByOwner) // if(not me)
	{
		IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);

		if (damageTakerActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;

			damageTakerActor->TakeDamage(damageData);
		}

		else
		{
			UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());

			if (mesh)
			{
				if (mesh->IsSimulatingPhysics())
				{
					FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
					forceVector.Normalize();
					//для одиночного толчка (испулься), лучше использовать AddImpulse().
					//mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
					// для постоянное воздействие на объект(например, для эффекта ветра или гравитации), лучше  AddForce().
					mesh->AddForce(forceVector * PushForce, NAME_None, true);
				}
			}
		}
	}
}
