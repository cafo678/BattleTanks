// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "CollisionQueryParams.h"
#include "Kismet/GameplayStatics.h"
#include "TankAimingComponent.generated.h"

// Enum declarations goes here
UENUM()
enum class EFiringStatus : uint8 
{
	Reloading, 
	Aiming, 
	Locked,
	OutOfAmmo
}; // ENUM for aiming state for crosshair color

class UTankBarrel; // Forward Declaration
class UTankTurret; // Forward Declaration
class AProjectile;

// Remember the code in the parenthesis to have this component available in the BP component addlist
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BT_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();
	
	UFUNCTION(Blueprintcallable, Category = Setup)
	void Fire();
	
	// Refactoring to set this component like the movement (without the extra passage) and not inherited (we can tweak details in the BP panel)
	UFUNCTION(Blueprintcallable, Category=Setting) 
	void Initialize (UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	UFUNCTION(Blueprintcallable, Category=Ammo) 
	int32 GetAmmoLeft();

	void AimAt(FVector AimLocation);
	
	EFiringStatus GetFiringState();

	float LastFireTime = 0;
	
protected:
	UPROPERTY (BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Reloading; // Declaration of a variable of the Enum

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint; //WE CHOOSE THE CLASS IN THE BP

private:
	UPROPERTY(EditAnywhere, Category = Firing) // Variable editable in blueprint
	float LaunchSpeed = 8000; // Speed of the projectile	

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	int32 AmmoLeft = 80;

	bool BarrelIsNotMoving();
	void MoveBarrel (FVector AimLocation); // Move the barrel in the direction we are aiming
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;	

	
	bool bIsReloaded = false;
	bool bHasAmmo = true;
	FVector AimDirection;
	
};
