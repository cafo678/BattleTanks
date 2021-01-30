// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Tank.h"
#include "MyAIController.h"

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();

    ControlledTank = GetPawn();
    PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
    AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
}

void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!ensure(PlayerTank || ControlledTank)) return;    

     MoveToActor(PlayerTank, AcceptanceRadius);
    
    if ( ( ControlledTank->GetActorLocation().GetAbs() - PlayerTank->GetActorLocation().GetAbs() ).Size() <= 10000.f)
    {
        if (!ensure(AimingComponent)) return;
        
        AimingComponent->AimAt(PlayerTank->GetActorLocation());
    
        if (AimingComponent->GetFiringState() == EFiringStatus::Locked)
        {
            AimingComponent->Fire();
        }     
    }
}

void AMyAIController::OnTankDeath()
{
    if (!GetPawn()) return;
    APawn* PawnToDestroy = GetPawn();
    GetPawn()->DetachFromControllerPendingDestroy();
    PawnToDestroy->Destroy();
}

void AMyAIController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);

    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);

        if (!ensure(PossessedTank)) return;
        
        PossessedTank->OnDeath.AddUniqueDynamic(this, &AMyAIController::OnTankDeath);
    }
}


