// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */

UCLASS( meta=(BlueprintSpawnableComponent) )
class BT_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	void Elevate(float RelativeSpeed);
	
private:
	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxDegreesPerSecond = 8;

	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxElevation = 35;

	UPROPERTY(EditAnywhere, Category = Setup)
	float MinElevation = -5;
};
