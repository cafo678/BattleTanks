// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bHasAmmo)
	{
		FiringStatus = EFiringStatus::OutOfAmmo;
	}
	else if (!((GetWorld()->GetTimeSeconds() - LastFireTime) > Cast<ATank>(GetOwner())->ReloadTimeInSeconds))
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (!BarrelIsNotMoving())
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else
	{
		FiringStatus = EFiringStatus::Locked;
	}	
}


// Called from BP to set the reference without pass from the Tank, refactoring
void UTankAimingComponent::Initialize (UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
    Barrel = BarrelToSet;
    Turret = TurretToSet;
}

// Aim, method called from PController
void UTankAimingComponent::AimAt(FVector AimLocation)
{
	if (!ensure(Barrel) || !ensure(Turret)) {return;} // ptr check

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile")); // Take the location of the socket
	
	if	
	(UGameplayStatics::SuggestProjectileVelocity // Proper unreal method for firing granades or balistic projectiles, need the class:: because is a static method
		(
		this, // use this because the aiming component is a WorldContextObject (parameter requested here)
		OutLaunchVelocity, // Out parameter with the velocity
		StartLocation, // start location
		AimLocation,  // End Location
		LaunchSpeed, // Speed of projectile
		false, // True if you want an high arc (wikipedia)
		0, // Radius of the projectile
		0, // if override the gravity, 0 means not
		ESuggestProjVelocityTraceOption::DoNotTrace // tracing options
		)
	)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();

		MoveBarrel(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrel (FVector AimDirection) // Move the barrel in the direction we are aiming
{
	auto ForwardRotator = Barrel->GetForwardVector().Rotation(); // Get the vector of the Barrel on axis X and make a FRotator from it
	auto AimAsRotator = AimDirection.Rotation(); // Get the vector of the AimDirection and make  a FRotator from it
	auto DeltaRotator = AimAsRotator - ForwardRotator;

	Barrel->Elevate(DeltaRotator.GetNormalized().Pitch);
	Turret->Rotate(DeltaRotator.GetNormalized().Yaw);
}

void UTankAimingComponent::Fire()
{
	bIsReloaded = (GetWorld()->GetTimeSeconds() - LastFireTime) > Cast<ATank>(GetOwner())->ReloadTimeInSeconds;
	
	if (bIsReloaded && bHasAmmo)
	{
		if (!ensure(Barrel && ProjectileBlueprint)) return;
		//This method return an actor of the type we spawned so we con store it and use it as a local variable
		auto Projectile = GetWorld()->SpawnActor<AProjectile>
		(
			ProjectileBlueprint, 
			Barrel->GetSocketLocation(FName("Projectile")), 
			Barrel->GetSocketRotation(FName("Projectile"))
		);
		
		//Calling the Launch method using that variable
		Projectile->LaunchProjectile(LaunchSpeed);

		LastFireTime = GetWorld()->GetTimeSeconds(); // Reset timer
		AmmoLeft--;

		if (AmmoLeft <= 0)
		{
			bHasAmmo = false;
		}
	}
}

bool UTankAimingComponent::BarrelIsNotMoving()
{
	return AimDirection.Equals(Barrel->GetForwardVector().GetSafeNormal(), 0.1);
}

EFiringStatus UTankAimingComponent::GetFiringState()
{
	return FiringStatus;
}

int32 UTankAimingComponent::GetAmmoLeft()
{
	return AmmoLeft;
}