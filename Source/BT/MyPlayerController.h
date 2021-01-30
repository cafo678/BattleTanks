// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

class UTankAimingComponent;

UCLASS()
class BT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float CrossHairXLocation = 0.5; // Divide the screensize for this for find the crosshair position

	UPROPERTY(EditAnywhere)
	float CrossHairYLocation = 0.33333; // Divide the screensize for this for find the crosshair position

	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000.f; // Multiplier for the traceline
	
	void AimTowardsCrosshair(); // Aim through the crosshair and see if hit something

	// Get a deprojected vector (normalized) based on the crosshair (OutLookDirection) and TraceLine in OutLookDirection and see if something is hit
	bool GetSightRayHitLocation(FVector& OutHitLocation) const; // Declaring a parameter by reference makes it an OUT Parameter

	// Find a location to aim
	bool GetLookDirection(FVector& OutWorldDirection) const; // Get LookDirection in normalized vector form (from -1 to 1) for every axis

	// Given the Deprojected crosshair vector, trace a line and see if something is Hit
	bool GetLookVectorHitLocation(FVector OutLookDirection, FVector& OutHitLocation) const;

	UTankAimingComponent* AimingComponent = nullptr;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnTankDeath();

public:
	// Check if there is a class BeginPlay in a parent class and use it (virtual and override are nedeed for calling functions that are of parent classes)
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
};
