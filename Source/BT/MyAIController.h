// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */

class UTankAimingComponent;

UCLASS()
class BT_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override; 

protected:
	UPROPERTY (EditDefaultsOnly, Category = Setup)
	float AcceptanceRadius = 6000.f; // How far from the goal this actor stops moving

private:
	UTankAimingComponent* AimingComponent = nullptr;
	APawn* PlayerTank = nullptr;
	APawn* ControlledTank = nullptr;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnTankDeath();
};
