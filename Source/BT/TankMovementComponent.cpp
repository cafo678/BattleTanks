// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "TankMovementComponent.h"

// Called from BP to set the reference without pass from the Tank
void UTankMovementComponent::Initialize (UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
        LeftTrack = LeftTrackToSet;
        RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward (float Throw)
{
    if (!ensure(LeftTrack && RightTrack)) return;
    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendMoveRight (float Throw)
{
    if (!ensure(LeftTrack && RightTrack)) return;
    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(-Throw);
}

// Velocity is a Vector that rapresent the direction
void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    FVector TankFacingDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
    FVector TankForwardIntentDirection = MoveVelocity.GetSafeNormal();

    // Making a DotProduct between tha direction of the Tank and the direction facing to the player, we use a cosine function to have a float between 0 and 1 for moving forward
    float AiThrowForward = FVector::DotProduct(TankFacingDirection, TankForwardIntentDirection);
    
    // Making a CrossProduct between tha direction of the Tank and the direction facing to the player, we use a sine function to have a vectore and extract the z float between 0 and 1 for moving right
    FVector AiThrowRight = FVector::CrossProduct(TankFacingDirection, TankForwardIntentDirection);

    IntendMoveForward(AiThrowForward);
    IntendMoveRight(AiThrowRight.Z);
}