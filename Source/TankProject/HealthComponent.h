// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStruct.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_EVENT(UHealthComponent, FOnDie)
	DECLARE_EVENT_OneParam(UHealthComponent, FOnHealthChanged, float)

public:
	// Sets default values for this component's properties
	UHealthComponent();

	void TakeDamage(FDamageData DamageData);
	float GetHealth() const;

	float GetHealthState() const;
	void AddHealth(float AddiditionalHealthValue);

	FOnDie OnDie;
	FOnHealthChanged OnDamaged;
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health values")
	float MaxHealth = 10;
	UPROPERTY()
	float CurrentHealth;

		
};
