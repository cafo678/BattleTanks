// Fill out your copyright notice in the Description page of Project Settings.
#include "TankAimingComponent.h"
#include "Tank.h"

// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = StartingHealth;
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{	
	int32 DamagePoints = FGenericPlatformMath::RoundToInt(DamageAmount);
	DamagePoints = FMath::Clamp(DamagePoints, 0, CurrentHealth);
	CurrentHealth -= DamagePoints;

	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
	}

	return DamageAmount;
}

float ATank::GetHealthPercent()
{
	return (float)CurrentHealth / (float)StartingHealth;
}


