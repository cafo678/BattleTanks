// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "SprungWheel.h"
#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Cosine function between the 2 vectors, 1 if the component is sliding right (-1 left), 0 if going Forward or Backward
    float SlippageSpeed = FVector::DotProduct(GetComponentVelocity(), GetRightVector());

    // The acceleration to correct the sliding, acceleration is velocity over time, DeltaTime for indipendent frame values
    FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
       
    // GetMass() requires a StaticMesh Component that is UNDER SceneComponent (obtained by GetRootComponent) in hierarcy, so simply cast then get the mass
    auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

    // Calculate and apply force (F = m a), but we have 2 tracks so we divide all per 2
    auto CorrectionForce = (CorrectionAcceleration * TankRoot->GetMass()) / 2;

    TankRoot->AddForceAtLocation(CorrectionForce, GetComponentLocation());
}

TArray<ASprungWheel*> UTankTrack::GetWheels()
{
    TArray<ASprungWheel*> ReturnArray;
    TArray<USceneComponent*> ChildrenArray;
    ASprungWheel* SprungWheel = nullptr;
    USpawnPoint* SpawnPoint = nullptr;

    GetChildrenComponents(true, ChildrenArray);

    for (USceneComponent* Child : ChildrenArray)
    {
        SpawnPoint = Cast<USpawnPoint>(Child); 
        if (!SpawnPoint) continue;

        SprungWheel = Cast<ASprungWheel>(SpawnPoint->GetSprungWheel());
        if (!SprungWheel) continue;

        ReturnArray.Add(SprungWheel);     
    }

    return ReturnArray;
}

void UTankTrack::SetThrottle (float Throttle)
{        
    Throttle = FMath::Clamp(Throttle, -1.f, 1.f);
    
    auto ForceApplied = Throttle * TrackMaxDrivingForce;
    auto Wheels = GetWheels();

    if (Wheels.Num() > 0)
    {
        auto ForcePerWheel = ForceApplied / Wheels.Num();
        
        for (ASprungWheel* Wheel : Wheels)
        {
            Wheel->AddDrivingForce(ForcePerWheel);
        }
    }
}

