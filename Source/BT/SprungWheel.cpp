// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"
#include "SprungWheel.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	ConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("ConstraintComponent"));
	SetRootComponent(ConstraintComponent);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(ConstraintComponent); // For attach a component ONLY IN THE CONSTRUCTOR

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle); // For attach a component ONLY IN THE CONSTRUCTOR

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	AxleWheelConstraint->SetupAttachment(Axle); // For attach a component ONLY IN THE CONSTRUCTOR
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

	SetConstraints();
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		ForceMagnitudeThisFrame = 0;
	}
	
}

void ASprungWheel::SetConstraints()
{
	// Get the Actor to wich the Component is Attached (the tank in this case)
	if (!ensure(GetAttachParentActor())) return;

	// We need a UPrimitiveComponent for the use of SetConstrainedComponents()
	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	if (!BodyRoot) return;

	// Set the constrain (the thing we edited in the BP) here
	ConstraintComponent->SetConstrainedComponents(BodyRoot, NAME_None, Wheel, NAME_None);
	AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	ForceMagnitudeThisFrame += ForceMagnitude;
}

void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ApplyForce();
}

void ASprungWheel::ApplyForce()
{
	Wheel->AddForce(Axle->GetForwardVector() * ForceMagnitudeThisFrame);
}
