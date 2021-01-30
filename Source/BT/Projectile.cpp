// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent> (FName("CollisionMesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true); // enable collision events by default

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent> (FName("ProjectileMovementComponent"));
	ProjectileMovementComponent->bAutoActivate = false;

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent> (FName("LaunchBlast"));
	LaunchBlast->bAutoActivate = true;
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent> (FName("ImpactBlast"));
	ImpactBlast->bAutoActivate = false;
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent> (FName("ExplosionForce"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AProjectile::LaunchProjectile(float LaunchSpeed)
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
	ProjectileMovementComponent->Activate();
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Because this is an actor we need to assign OnComponentHit to CollisionMesh, if this was a component that's not needed
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();

	ExplosionForce->FireImpulse();

	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	UGameplayStatics::ApplyRadialDamage
	(
		this,
		ProjectileDamage,
		GetActorLocation(),
		ExplosionForce->Radius, // the radius will be the same as the BP SETTING
		UDamageType::StaticClass(),
		TArray<AActor*>() // actor that take damage, this way all actors but be sure to delete the mesh of the proj before
	);

	FTimerHandle ProjectileTimer;
	GetWorld()->GetTimerManager().SetTimer(ProjectileTimer, this, &AProjectile::DestroyActor, TimerDelay, false);
}

void AProjectile::DestroyActor()
{
	Destroy();
}