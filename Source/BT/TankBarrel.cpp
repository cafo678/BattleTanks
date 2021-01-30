// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
    RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.f, 1.f);
    auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
    auto NewElevation = FMath::Clamp(RawNewElevation, MinElevation, MaxElevation);
    SetRelativeRotation(FRotator(NewElevation, 0, 0));
}

