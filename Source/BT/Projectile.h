// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystemComponent;
class URadialForceComponent;

UCLASS()
class BT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void BeginPlay() override;

	void LaunchProjectile(float LaunchSpeed);

private:	
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr; // Now the Tank has a reference to the Projectile Class

	UPROPERTY(EditDefaultsOnly)
	float TimerDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileDamage = 20.0f;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CollisionMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* LaunchBlast = nullptr;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ImpactBlast = nullptr;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ExplosionForce = nullptr;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyActor();
};
