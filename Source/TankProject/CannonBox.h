// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CannonBox.generated.h"

UCLASS()
class TANKPROJECT_API ACannonBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannonBox();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonBoxMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<class ACannon> CannonClass;

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
