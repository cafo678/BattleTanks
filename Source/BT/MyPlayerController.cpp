// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "MyPlayerController.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (ensure(GetPawn())) 
    {
        AimTowardsCrosshair(); // Aim through the crosshair and see if hit something
    }
}

void AMyPlayerController::AimTowardsCrosshair() // Aim through the crosshair and see if hit something
{
    AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

    if (!ensure(AimingComponent)) return;

    FVector OutHitLocation; // Declare a OUT parameter
    
    if  (GetSightRayHitLocation(OutHitLocation)) // If found a location to aim ...
    {
        AimingComponent->AimAt(OutHitLocation); // ... Aim at that location
    }
}

// Get a deprojected vector (normalized) based on the crosshair (OutLookDirection) and TraceLine in OutLookDirection and see if something is hit
bool AMyPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
    FVector OutLookDirection; // Works from 0 to 1 for every axis, gives Look Direction

    if (GetLookDirection(OutLookDirection)) // Get a deprojected vector (normalized) based on the crosshair
    {
        return GetLookVectorHitLocation(OutLookDirection, OutHitLocation); // TraceLine in OutLookDirection and see if something is hit
    }
    
    return false;
}

bool AMyPlayerController::GetLookDirection(FVector& OutLookDirection) const // Get a deprojected vector (normalized) based on the crosshair
{
    // Finding the position of the crosshair on the screen

    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);
    
    FVector OutCameraLocation; // Needed by the next method but not used

    // Deproject the crosshair screen position in the world, OutLookDirection is a normalized vector, from -1 to 1 for every axis
    return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, OutCameraLocation, OutLookDirection);
}

// Find a location to aim
// Given the Deprojected crosshair vector, trace a line and see if something is Hit, OutHitLocation = Location of the hitted Object
bool AMyPlayerController::GetLookVectorHitLocation(FVector OutLookDirection, FVector& OutHitLocation) const
{
    FVector StartingTracePoint = PlayerCameraManager->GetCameraLocation(); // Start the trace at the camera location
    FVector EndingTracePoint = StartingTracePoint + OutLookDirection * LineTraceRange; // Multiply the LookDirection for the Range decided

    FHitResult OutHittedObject; // Declare an OUT parameter
    
    // If hitted something ...
    if (GetWorld() -> LineTraceSingleByChannel
            (
            OutHittedObject, // FHitResult of the hitted object
            StartingTracePoint, // Start the line here
            EndingTracePoint, // End the line here
            ECC_Visibility, // Filter the object by visibility
            FCollisionQueryParams(),
            FCollisionResponseParams()
            )
        )
    // ... Set the OutHitLocation
    {
        OutHitLocation = OutHittedObject.Location;
        return true;
    }

    return false; 
}

void AMyPlayerController::OnTankDeath()
{
    UGameplayStatics::OpenLevel(GetWorld(), "DeathMenu");
}

void AMyPlayerController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);

    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);

        if (!ensure(PossessedTank)) return;
        
        PossessedTank->OnDeath.AddUniqueDynamic(this, &AMyPlayerController::OnTankDeath);
    }
}