// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 * 
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BT_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	// Bluprint callable for firing this event in BP for input
	UFUNCTION(Blueprintcallable, Category=Input)
	void IntendMoveForward (float Throw); // Throw is the "run" of the controller's stick

	UFUNCTION(Blueprintcallable, Category=Input)
	void IntendMoveRight (float Throw);

	UFUNCTION(Blueprintcallable, Category=Setting)
	void Initialize (UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet);

	// Called from the pathfinding logic by standard AIController
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	
private:
	// For having a reference to the tracks, we won't pass from the Tank anymore
	UTankTrack* LeftTrack = nullptr;
	UTankTrack* RightTrack = nullptr;
};
