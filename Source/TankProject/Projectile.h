#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	
	void Start();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveRate = 0.005f; // position update frequency

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1;

	FTimerHandle MovementTimer;

	UFUNCTION()
	void Move();

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
