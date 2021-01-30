// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */
UCLASS( meta=(BlueprintSpawnableComponent) )
class BT_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UFUNCTION (BlueprintCallable, Category = Input)
	void SetThrottle (float Throttle);

	// Max Force per track in Newton, calculated by the teacher
	UPROPERTY(EditDefaultsOnly, Category = Moving)
	float TrackMaxDrivingForce = 400000000000000;

private:
	TArray<class ASprungWheel*> GetWheels();

	UTankTrack(); // Constructor for ticking

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
};
